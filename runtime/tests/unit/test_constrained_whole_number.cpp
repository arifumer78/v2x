#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;

TEST(ConstrainedWholeNumber, InvalidConstraintWhenLbGreaterThanUb) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = w.constrained_whole_number(0, 10, 5);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::InvalidConstraint);
}

TEST(ConstrainedWholeNumber, ValueBelowLowerBoundFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = w.constrained_whole_number(4, 5, 10);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::ValueOutOfRange);
}

TEST(ConstrainedWholeNumber, ValueAboveUpperBoundFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = w.constrained_whole_number(11, 5, 10);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::ValueOutOfRange);
}

TEST(ConstrainedWholeNumber, RangeOneWritesZeroBits) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(5, 5, 5));
    EXPECT_EQ(w.bits_written(), 0u);
}

TEST(ConstrainedWholeNumber, RangeOneDecodesWithoutConsumingBits) {
    std::array<std::byte, 4> buf{};
    PerReader<Unaligned> r(buf);
    auto result = r.constrained_whole_number(7, 7);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 7);
    EXPECT_EQ(r.bits_remaining(), 32u);
}

TEST(ConstrainedWholeNumber, RangeTwoWritesOneBit) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(1, 0, 1));
    EXPECT_EQ(w.bits_written(), 1u);
    EXPECT_EQ(buf[0], std::byte{0x80});
}

TEST(ConstrainedWholeNumber, EightBitFullRangeKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(255, 0, 255)); // range=256, span=255, bit_width=8
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0xFF});
}

TEST(ConstrainedWholeNumber, BitWidthTransition255To256) {
    // ub=255 -> span=255 -> bit_width=8; ub=256 -> span=256 -> bit_width=9.
    std::array<std::byte, 2> buf8{};
    PerWriter<Unaligned> w8(buf8);
    ASSERT_TRUE(w8.constrained_whole_number(0, 0, 255));
    EXPECT_EQ(w8.bits_written(), 8u);

    std::array<std::byte, 2> buf9{};
    PerWriter<Unaligned> w9(buf9);
    ASSERT_TRUE(w9.constrained_whole_number(0, 0, 256));
    EXPECT_EQ(w9.bits_written(), 9u);
}

TEST(ConstrainedWholeNumber, FixedWidthToFallbackBoundary65536To65537) {
    // ub=65535 -> range=65536, span=65535, bit_width=16 -> still fixed-width.
    std::array<std::byte, 4> buf_fixed{};
    PerWriter<Unaligned> w_fixed(buf_fixed);
    ASSERT_TRUE(w_fixed.constrained_whole_number(65535, 0, 65535));
    EXPECT_EQ(w_fixed.bits_written(), 16u);

    // ub=65536 -> range=65537, span=65536, bit_width=17 -> fallback path.
    // offset=65536 needs ceil(17/8)=3 octets -> 8 (length) + 24 (octets) = 32 bits.
    std::array<std::byte, 8> buf_fallback{};
    PerWriter<Unaligned> w_fallback(buf_fallback);
    ASSERT_TRUE(w_fallback.constrained_whole_number(65536, 0, 65536));
    EXPECT_EQ(w_fallback.bits_written(), 32u);
}

TEST(ConstrainedWholeNumber, RoundTripFixedWidth) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(42, 10, 1000));

    PerReader<Unaligned> r(buf);
    auto result = r.constrained_whole_number(10, 1000);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 42);
}

TEST(ConstrainedWholeNumber, RoundTripFallbackPath) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(100000, 0, 200000)); // span=200000 > 65535 -> fallback

    PerReader<Unaligned> r(buf);
    auto result = r.constrained_whole_number(0, 200000);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 100000);
}

TEST(ConstrainedWholeNumber, RoundTripFullInt64Range) {
    // lb=INT64_MIN, ub=INT64_MAX: span computed via uint64_t wraps to UINT64_MAX,
    // exercising the overflow-safe span/offset arithmetic directly.
    constexpr int64_t lb = std::numeric_limits<int64_t>::min();
    constexpr int64_t ub = std::numeric_limits<int64_t>::max();
    constexpr int64_t v = 123456789;

    std::array<std::byte, 16> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.constrained_whole_number(v, lb, ub));

    PerReader<Unaligned> r(buf);
    auto result = r.constrained_whole_number(lb, ub);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), v);
}

TEST(ConstrainedWholeNumber, DecodeRejectsOffsetBeyondSpan) {
    // lb=0, ub=2 -> span=2, bit_width=2, valid offsets {0,1,2}. Hand-write the
    // out-of-range raw pattern 0b11 (offset=3) and confirm the decoder rejects it.
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true));

    PerReader<Unaligned> r(buf);
    auto result = r.constrained_whole_number(0, 2);
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::MalformedEncoding);
}

TEST(ConstrainedWholeNumber, BufferExhaustedOnWrite) {
    std::array<std::byte, 1> buf{}; // 8 bits total
    PerWriter<Unaligned> w(buf);
    auto status = w.constrained_whole_number(500, 0, 1000); // needs 10 bits
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}
