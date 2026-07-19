#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;

// X.691 §11.6: n<=63 -> flag bit 0 + n itself (not n-1) as a 6-bit field.
// n>=64 -> flag bit 1 + n as a semi-constrained whole number (lb=0) — the
// same length-prefixed-minimal-octets shape as semi_constrained_whole_number
// itself, with no upper limit on n. (An earlier version of this file tested
// a different, spec-incorrect implementation — see docs/CHANGELOG.md.)

TEST(NormallySmallLength, SmallFormZeroKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.normally_small_length(0));
    EXPECT_EQ(w.bits_written(), 7u);
    EXPECT_EQ(buf[0], std::byte{0x00}); // flag 0, then 6 bits of 0
}

TEST(NormallySmallLength, SmallFormBoundary63KnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.normally_small_length(63));
    EXPECT_EQ(w.bits_written(), 7u);
    EXPECT_EQ(buf[0], std::byte{0x7E}); // flag 0, then 6 bits of 63 = 0b111111
}

TEST(NormallySmallLength, LargeFormBoundary64KnownAnswer) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.normally_small_length(64));
    // flag(1) + length-of-octet-count byte (1) + content octet (64=0x40) = 17 bits
    EXPECT_EQ(w.bits_written(), 17u);
    EXPECT_EQ(buf[0], std::byte{0x80});
    EXPECT_EQ(buf[1], std::byte{0xA0});
    EXPECT_EQ(buf[2], std::byte{0x00});
}

TEST(NormallySmallLength, NoUpperLimitOnLargeForm) {
    // The earlier (spec-incorrect) implementation rejected values requiring
    // "fragmentation" above a length_determinant-style threshold. The
    // correct §11.6.2 shape (semi-constrained whole number) has no such
    // limit — large values just take more octets.
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.normally_small_length(1000000));
}

TEST(NormallySmallLength, RoundTripSmallForm) {
    for (uint32_t n : {0u, 1u, 30u, 63u}) {
        std::array<std::byte, 1> buf{};
        PerWriter<Unaligned> w(buf);
        ASSERT_TRUE(w.normally_small_length(n));

        PerReader<Unaligned> r(buf);
        auto result = r.normally_small_length();
        ASSERT_TRUE(result.ok());
        EXPECT_EQ(result.value(), n);
    }
}

TEST(NormallySmallLength, RoundTripLargeForm) {
    for (uint32_t n : {64u, 65u, 1000u, 16000u, 1000000u}) {
        std::array<std::byte, 8> buf{};
        PerWriter<Unaligned> w(buf);
        ASSERT_TRUE(w.normally_small_length(n));

        PerReader<Unaligned> r(buf);
        auto result = r.normally_small_length();
        ASSERT_TRUE(result.ok());
        EXPECT_EQ(result.value(), n);
    }
}

TEST(NormallySmallLength, DecodeMalformedTopBitSetInLargeFormLengthPrefix) {
    // Large form's length prefix reuses write_short_length_prefix_'s
    // short-form-only contract (top bit must be 0) — same malformed-input
    // check as semi_constrained_whole_number/unconstrained_whole_number.
    // Hand-craft: flag bit true, then a length-prefix byte with top bit set.
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bits(std::array<std::byte, 1>{std::byte{0x80}}, 8));

    PerReader<Unaligned> r(buf);
    auto result = r.normally_small_length();
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::MalformedEncoding);
}
