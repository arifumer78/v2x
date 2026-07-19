#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

// Known-answer tests transcribed directly from ITU-T Recommendation X.691
// (02/2021) — not hand-derived — as an independent cross-check on top of the
// Layer-1 tests written alongside each primitive's implementation. Each test
// cites the exact clause/annex and quotes the relevant spec text so the
// source is traceable.

using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;

// Annex A.1 ("Example of encodings") defines a PersonnelRecord with
// `EmployeeNumber ::= [APPLICATION 2] IMPLICIT INTEGER` (unconstrained) and
// a sample value of 51. A.1.4.2 ("UNALIGNED PER representation") shows:
//   0.0000001   Length of (employee) number = 1
//   0.0110011   (employee) number = 51
// i.e. a single length-prefix octet 0x01 followed by a single content octet
// 0x33 (51 decimal) — exactly the shape unconstrained_whole_number produces.
TEST(X691SpecExamples, UnconstrainedWholeNumber_PersonnelRecordEmployeeNumber_AnnexA1) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(51));
    EXPECT_EQ(w.bits_written(), 16u);
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0x33});

    PerReader<Unaligned> r(buf);
    auto result = r.unconstrained_whole_number();
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 51);
}

// §11.9.3.6, worked NOTE: "if ... a value of A is 4 characters long ... both
// values are encoded with the length octet occupying one octet, and with
// the most significant [bit] set to 0":
//   0 0000100   4 characters/items
TEST(X691SpecExamples, LengthDeterminant_ShortForm_Clause11_9_3_6) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto result = w.length_determinant(4, std::nullopt);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 4u);
    EXPECT_EQ(buf[0], std::byte{0x04});
}

// §11.9.3.7, worked NOTE: "if ... a value of A is 130 characters long ...
// both values are encoded with the length component occupying 2 octets,
// and with the two most significant bits ... set to 10":
//   10 000000 10000010   130 characters/items
TEST(X691SpecExamples, LengthDeterminant_MediumForm_Clause11_9_3_7) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    auto result = w.length_determinant(130, std::nullopt);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 130u);
    EXPECT_EQ(buf[0], std::byte{0x80});
    EXPECT_EQ(buf[1], std::byte{0x82});
}

// §11.9.3.8.1, worked NOTE 2: "if ... a value of 'B' is 144K + 1 (i.e., 64K +
// 64K + 16K + 1) items long, the value is fragmented, with the two most
// significant bits of the first three fragments set to 11 ...":
//   11 000100  64K items   11 000100  64K items   11 000001  16K items   0 0000001  1 item
// i.e. chunks [65536, 65536, 16384, 1] with markers 0xC4, 0xC4, 0xC1 and a
// final short-form terminator 0x01. This is the single highest-risk piece of
// the whole kernel (recursive/stateful fragmentation logic per the design
// doc's own risk list) — matching the spec's own worked example exactly is
// meaningfully stronger evidence than the hand-constructed fragmentation
// tests written alongside the implementation.
TEST(X691SpecExamples, LengthDeterminant_Fragmentation_144KPlus1_Clause11_9_3_8) {
    constexpr uint32_t kTotal = 64u * 1024u + 64u * 1024u + 16u * 1024u + 1u; // 147457
    // No content is actually written here (length_determinant alone is under
    // test — see the comment below), so only the 4 marker/terminator bytes
    // are needed, not kTotal bytes.
    std::array<std::byte, 16> buf{};
    PerWriter<Unaligned> w(buf);

    std::vector<uint32_t> chunks;
    uint32_t remaining = kTotal;
    while (true) {
        auto ld = w.length_determinant(remaining, std::nullopt);
        ASSERT_TRUE(ld.ok());
        const uint32_t chunk = ld.value();
        chunks.push_back(chunk);
        remaining -= chunk;
        if (chunk < 16384) {
            break;
        }
    }

    const std::vector<uint32_t> expected_chunks{65536u, 65536u, 16384u, 1u};
    EXPECT_EQ(chunks, expected_chunks);

    // This test drives length_determinant alone (no accompanying content
    // writes — see LengthDeterminant.Fragmentation* in test_length_determinant.cpp
    // for the version that interleaves real content, mirroring how open_type
    // actually uses this), so the four marker/terminator bytes sit back to
    // back at the front of the buffer, per the spec's own annotated layout.
    EXPECT_EQ(buf[0], std::byte{0xC4}); // 11 000100 -> k=4 (64K)
    EXPECT_EQ(buf[1], std::byte{0xC4}); // 11 000100 -> k=4 (64K)
    EXPECT_EQ(buf[2], std::byte{0xC1}); // 11 000001 -> k=1 (16K)
    EXPECT_EQ(buf[3], std::byte{0x01}); // 0 0000001 -> n=1, short form
}
