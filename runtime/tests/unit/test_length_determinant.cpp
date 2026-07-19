#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/size_range.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::SizeRange;
using v2x::per::Unaligned;

TEST(LengthDeterminant, ShortFormZeroKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(0, std::nullopt);
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 0u);
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0x00});
}

TEST(LengthDeterminant, ShortFormBoundary127) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(127, std::nullopt);
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 127u);
    EXPECT_EQ(buf[0], std::byte{0x7F});
}

TEST(LengthDeterminant, MediumFormBoundary128) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(128, std::nullopt);
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 128u);
    EXPECT_EQ(w.bits_written(), 16u);
    EXPECT_EQ(buf[0], std::byte{0x80});
    EXPECT_EQ(buf[1], std::byte{0x80});
}

TEST(LengthDeterminant, MediumFormBoundary16383) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(16383, std::nullopt);
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 16383u);
    EXPECT_EQ(buf[0], std::byte{0xBF});
    EXPECT_EQ(buf[1], std::byte{0xFF});
}

TEST(LengthDeterminant, FragmentationBoundary16384SingleMarker) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(16384, std::nullopt);
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 16384u); // one full 16384-chunk marker
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0xC1}); // top bits '11', k=1
}

TEST(LengthDeterminant, FragmentationTwoFullChunks) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(32768, std::nullopt); // exactly 2*16384
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 32768u);
    EXPECT_EQ(buf[0], std::byte{0xC2}); // k=2
}

TEST(LengthDeterminant, FragmentationCapsAtFourChunks) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(200000, std::nullopt); // far more than 4*16384
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 4u * 16384u);
    EXPECT_EQ(buf[0], std::byte{0xC4}); // k capped at 4
}

// Drives the real X.691 fragmentation protocol: keep calling length_determinant
// with the remaining count until a call returns a chunk < 16384 (short/medium
// form) — that's the terminator, even when the running remainder hits exactly
// zero after a full chunk. Mirrors what the future SEQUENCE OF/OCTET STRING
// orchestration layer will do, interleaving actual content between calls.
std::vector<uint32_t> DriveWriterFragments(PerWriter<Unaligned>& w, uint32_t n) {
    std::vector<uint32_t> chunks;
    uint32_t remaining = n;
    while (true) {
        auto r = w.length_determinant(remaining, std::nullopt);
        if (!r.ok()) {
            ADD_FAILURE() << "length_determinant failed mid-fragmentation";
            return chunks;
        }
        const uint32_t chunk = r.value();
        chunks.push_back(chunk);
        remaining -= chunk;
        if (chunk < 16384) {
            break;
        }
    }
    return chunks;
}

std::vector<uint32_t> DriveReaderFragments(PerReader<Unaligned>& r) {
    std::vector<uint32_t> chunks;
    while (true) {
        auto res = r.length_determinant(std::nullopt);
        if (!res.ok()) {
            ADD_FAILURE() << "length_determinant failed mid-fragmentation";
            return chunks;
        }
        const uint32_t chunk = res.value();
        chunks.push_back(chunk);
        if (chunk < 16384) {
            break;
        }
    }
    return chunks;
}

TEST(LengthDeterminant, FragmentationCompositionSumsToN16384) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto chunks = DriveWriterFragments(w, 16384);
    uint32_t total = 0;
    for (auto c : chunks) {
        total += c;
    }
    EXPECT_EQ(total, 16384u);
    EXPECT_EQ(chunks.back(), 0u); // exact multiple of 16384 still needs a zero terminator

    PerReader<Unaligned> r(buf);
    auto read_chunks = DriveReaderFragments(r);
    EXPECT_EQ(read_chunks, chunks);
}

TEST(LengthDeterminant, FragmentationCompositionSumsToN16385) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto chunks = DriveWriterFragments(w, 16385);
    uint32_t total = 0;
    for (auto c : chunks) {
        total += c;
    }
    EXPECT_EQ(total, 16385u);

    PerReader<Unaligned> r(buf);
    auto read_chunks = DriveReaderFragments(r);
    EXPECT_EQ(read_chunks, chunks);
}

TEST(LengthDeterminant, FragmentationCompositionSumsToN70000) {
    // 70000 > 4*16384 (65536): needs a capped-at-4 marker, then a remainder marker, then a terminator.
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    auto chunks = DriveWriterFragments(w, 70000);
    uint32_t total = 0;
    for (auto c : chunks) {
        total += c;
    }
    EXPECT_EQ(total, 70000u);
    ASSERT_GE(chunks.size(), 2u);

    PerReader<Unaligned> r(buf);
    auto read_chunks = DriveReaderFragments(r);
    EXPECT_EQ(read_chunks, chunks);
}

TEST(LengthDeterminant, ConstrainedDelegationKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    const SizeRange constraint{1, 10};
    auto r = w.length_determinant(5, constraint); // span=9, bit_width=4; offset=5-1=4=0b0100
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value(), 5u);
    EXPECT_EQ(w.bits_written(), 4u);
}

TEST(LengthDeterminant, ConstrainedDelegationOutOfRangeFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const SizeRange constraint{1, 10};
    auto r = w.length_determinant(20, constraint);
    ASSERT_FALSE(r.ok());
    EXPECT_EQ(r.error(), Error::LengthOutOfRange);
}

TEST(LengthDeterminant, ConstrainedDelegationRoundTrip) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    const SizeRange constraint{1, 10};
    ASSERT_TRUE(w.length_determinant(7, constraint));

    PerReader<Unaligned> r(buf);
    auto result = r.length_determinant(constraint);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 7u);
}

TEST(LengthDeterminant, RoundTripShortForm) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.length_determinant(42, std::nullopt));

    PerReader<Unaligned> r(buf);
    auto result = r.length_determinant(std::nullopt);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 42u);
}

TEST(LengthDeterminant, RoundTripMediumForm) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.length_determinant(5000, std::nullopt));

    PerReader<Unaligned> r(buf);
    auto result = r.length_determinant(std::nullopt);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 5000u);
}

TEST(LengthDeterminant, BufferExhaustedOnWrite) {
    std::array<std::byte, 0> buf{};
    PerWriter<Unaligned> w(buf);
    auto r = w.length_determinant(5, std::nullopt);
    ASSERT_FALSE(r.ok());
    EXPECT_EQ(r.error(), Error::BufferExhausted);
}
