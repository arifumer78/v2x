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

TEST(SemiConstrainedWholeNumber, ValueBelowLowerBoundFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = w.semi_constrained_whole_number(4, 5);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::ValueOutOfRange);
}

TEST(SemiConstrainedWholeNumber, OffsetZeroKnownAnswer) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.semi_constrained_whole_number(100, 100)); // offset=0
    EXPECT_EQ(w.bits_written(), 16u);
    EXPECT_EQ(buf[0], std::byte{0x01}); // length prefix: 1 octet
    EXPECT_EQ(buf[1], std::byte{0x00});
}

TEST(SemiConstrainedWholeNumber, OffsetFiveKnownAnswer) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.semi_constrained_whole_number(5, 0)); // offset=5
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0x05});
}

TEST(SemiConstrainedWholeNumber, OctetCountTransition255To256) {
    std::array<std::byte, 4> buf255{};
    PerWriter<Unaligned> w255(buf255);
    ASSERT_TRUE(w255.semi_constrained_whole_number(255, 0));
    EXPECT_EQ(w255.bits_written(), 16u); // 8 (length) + 8 (1 octet)

    std::array<std::byte, 4> buf256{};
    PerWriter<Unaligned> w256(buf256);
    ASSERT_TRUE(w256.semi_constrained_whole_number(256, 0));
    EXPECT_EQ(w256.bits_written(), 24u); // 8 (length) + 16 (2 octets)
}

TEST(SemiConstrainedWholeNumber, OctetCountTransition65535To65536) {
    std::array<std::byte, 8> buf1{};
    PerWriter<Unaligned> w1(buf1);
    ASSERT_TRUE(w1.semi_constrained_whole_number(65535, 0));
    EXPECT_EQ(w1.bits_written(), 24u); // 8 + 16

    std::array<std::byte, 8> buf2{};
    PerWriter<Unaligned> w2(buf2);
    ASSERT_TRUE(w2.semi_constrained_whole_number(65536, 0));
    EXPECT_EQ(w2.bits_written(), 32u); // 8 + 24
}

TEST(SemiConstrainedWholeNumber, RoundTrip) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.semi_constrained_whole_number(987654, -1000));

    PerReader<Unaligned> r(buf);
    auto result = r.semi_constrained_whole_number(-1000);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 987654);
}

TEST(SemiConstrainedWholeNumber, RoundTripOffsetZero) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.semi_constrained_whole_number(42, 42));

    PerReader<Unaligned> r(buf);
    auto result = r.semi_constrained_whole_number(42);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 42);
}

TEST(SemiConstrainedWholeNumber, DecodeMalformedTopBitSetInLength) {
    std::array<std::byte, 2> buf{std::byte{0x80}, std::byte{0x00}}; // top bit set: not short form
    PerReader<Unaligned> r(buf);
    auto result = r.semi_constrained_whole_number(0);
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::MalformedEncoding);
}
