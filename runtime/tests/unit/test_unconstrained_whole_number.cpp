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

TEST(UnconstrainedWholeNumber, ZeroKnownAnswer) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(0));
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0x00});
}

TEST(UnconstrainedWholeNumber, MinusOneKnownAnswer) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(-1));
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0xFF});
}

TEST(UnconstrainedWholeNumber, OneOctetBoundary127) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(127));
    EXPECT_EQ(w.bits_written(), 16u); // 8 (length) + 8 (1 octet)
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0x7F});
}

TEST(UnconstrainedWholeNumber, TwoOctetBoundary128) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(128));
    EXPECT_EQ(w.bits_written(), 24u); // 8 + 16
    EXPECT_EQ(buf[0], std::byte{0x02});
    EXPECT_EQ(buf[1], std::byte{0x00});
    EXPECT_EQ(buf[2], std::byte{0x80});
}

TEST(UnconstrainedWholeNumber, OneOctetBoundaryMinus128) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(-128));
    EXPECT_EQ(w.bits_written(), 16u);
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{0x80});
}

TEST(UnconstrainedWholeNumber, TwoOctetBoundaryMinus129) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.unconstrained_whole_number(-129));
    EXPECT_EQ(w.bits_written(), 24u);
    EXPECT_EQ(buf[0], std::byte{0x02});
    EXPECT_EQ(buf[1], std::byte{0xFF});
    EXPECT_EQ(buf[2], std::byte{0x7F});
}

TEST(UnconstrainedWholeNumber, RoundTripFullInt64Range) {
    constexpr std::array<int64_t, 6> values{
        0,
        1,
        -1,
        std::numeric_limits<int64_t>::max(),
        std::numeric_limits<int64_t>::min(),
        123456789012345,
    };
    for (int64_t v : values) {
        std::array<std::byte, 9> buf{};
        PerWriter<Unaligned> w(buf);
        ASSERT_TRUE(w.unconstrained_whole_number(v));

        PerReader<Unaligned> r(buf);
        auto result = r.unconstrained_whole_number();
        ASSERT_TRUE(result.ok());
        EXPECT_EQ(result.value(), v);
    }
}

TEST(UnconstrainedWholeNumber, DecodeMalformedTopBitSetInLength) {
    std::array<std::byte, 2> buf{std::byte{0x80}, std::byte{0x00}};
    PerReader<Unaligned> r(buf);
    auto result = r.unconstrained_whole_number();
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::MalformedEncoding);
}

TEST(UnconstrainedWholeNumber, BufferExhaustedOnWrite) {
    std::array<std::byte, 1> buf{}; // 8 bits: only room for the length byte
    PerWriter<Unaligned> w(buf);
    auto status = w.unconstrained_whole_number(1000);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}
