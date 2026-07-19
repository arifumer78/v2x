#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <optional>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/octet_string.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/size_range.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_octet_string;
using v2x::per::SizeRange;
using v2x::per::Unaligned;
using v2x::per::write_octet_string;

TEST(OctetString, EmptyUnconstrainedKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_octet_string(w, std::span<const std::byte>{}));
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0x00});
}

TEST(OctetString, SmallUnconstrainedKnownAnswer) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xAB}, std::byte{0xCD}};
    ASSERT_TRUE(write_octet_string(w, content));
    EXPECT_EQ(w.bits_written(), 24u); // 8 (length) + 16 (content)
    EXPECT_EQ(buf[0], std::byte{0x02});
    EXPECT_EQ(buf[1], std::byte{0xAB});
    EXPECT_EQ(buf[2], std::byte{0xCD});
}

TEST(OctetString, FixedSizeConstraintCollapsesToZeroLengthBits) {
    std::array<std::byte, 5> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 5> content{std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}, std::byte{5}};
    const SizeRange constraint{5, 5};
    ASSERT_TRUE(write_octet_string(w, content, constraint));
    EXPECT_EQ(w.bits_written(), 40u); // 0 (fixed-size length, span=0) + 40 (content)
}

TEST(OctetString, RangeSizeConstraintKnownAnswer) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 3> content{std::byte{1}, std::byte{2}, std::byte{3}};
    const SizeRange constraint{0, 10}; // span=10, bit_width=4
    ASSERT_TRUE(write_octet_string(w, content, constraint));
    EXPECT_EQ(w.bits_written(), 4u + 24u);
}

TEST(OctetString, SizeConstraintOutOfRangeFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 3> content{};
    const SizeRange constraint{5, 10};
    auto status = write_octet_string(w, content, constraint);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::LengthOutOfRange);
}

TEST(OctetString, RoundTripUnconstrained) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 4> content{std::byte{0x11}, std::byte{0x22}, std::byte{0x33}, std::byte{0x44}};
    ASSERT_TRUE(write_octet_string(w, content));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 4> out{};
    auto result = read_octet_string(r, out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 4u);
    EXPECT_EQ(out, content);
}

TEST(OctetString, RoundTripSizeConstrained) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 3> content{std::byte{9}, std::byte{8}, std::byte{7}};
    const SizeRange constraint{0, 10};
    ASSERT_TRUE(write_octet_string(w, content, constraint));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 3> out{};
    auto result = read_octet_string(r, out, constraint);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u);
    EXPECT_EQ(out, content);
}

TEST(OctetString, FragmentationRoundTrip) {
    std::vector<std::byte> content(20000);
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>(i % 256);
    }
    std::vector<std::byte> buf(20100);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_octet_string(w, content));

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(20000);
    auto result = read_octet_string(r, out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 20000u);
    EXPECT_EQ(out, content);
}

TEST(OctetString, ReaderDestinationTooSmallFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 3> content{};
    ASSERT_TRUE(write_octet_string(w, content));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 1> out{};
    auto result = read_octet_string(r, out);
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::BufferExhausted);
}
