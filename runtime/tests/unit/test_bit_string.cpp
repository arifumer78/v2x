#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <optional>
#include <vector>

#include "v2x/per/bit_string.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/size_range.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_bit_string;
using v2x::per::SizeRange;
using v2x::per::Unaligned;
using v2x::per::write_bit_string;

TEST(BitString, ZeroLengthKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_bit_string(w, std::span<const std::byte>{}, 0));
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0x00});
}

TEST(BitString, NonByteAlignedThirteenBitsKnownAnswer) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    // byte0 = 10110100, byte1's top 5 bits = 10100, rest zero-padded.
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    ASSERT_TRUE(write_bit_string(w, content, 13));
    EXPECT_EQ(w.bits_written(), 21u); // 8 (length) + 13 (content)
    EXPECT_EQ(buf[0], std::byte{0x0D}); // length_determinant(13) short form
    EXPECT_EQ(buf[1], std::byte{0xB4});
    EXPECT_EQ(buf[2], std::byte{0xA0});
}

TEST(BitString, ByteAlignmentBoundary16Vs17Bits) {
    std::array<std::byte, 4> buf16{};
    PerWriter<Unaligned> w16(buf16);
    const std::array<std::byte, 2> content16{std::byte{0xFF}, std::byte{0xFF}};
    ASSERT_TRUE(write_bit_string(w16, content16, 16));
    EXPECT_EQ(w16.bits_written(), 24u); // 8 + 16

    std::array<std::byte, 4> buf17{};
    PerWriter<Unaligned> w17(buf17);
    const std::array<std::byte, 3> content17{std::byte{0xFF}, std::byte{0xFF}, std::byte{0x80}};
    ASSERT_TRUE(write_bit_string(w17, content17, 17));
    EXPECT_EQ(w17.bits_written(), 25u); // 8 + 17
}

TEST(BitString, SizeConstrainedKnownAnswer) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    const SizeRange constraint{1, 20}; // span=19, bit_width=5
    ASSERT_TRUE(write_bit_string(w, content, 13, constraint));
    EXPECT_EQ(w.bits_written(), 5u + 13u);
}

TEST(BitString, SizeConstraintOutOfRangeFails) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 1> content{};
    const SizeRange constraint{5, 10};
    auto status = write_bit_string(w, content, 2, constraint); // 2 bits, outside [5,10]
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::LengthOutOfRange);
}

TEST(BitString, RoundTripNonByteAligned) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    ASSERT_TRUE(write_bit_string(w, content, 13));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 2> out{};
    auto result = read_bit_string(r, out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 13u); // bits, not bytes
    EXPECT_EQ(out, content);
}

TEST(BitString, RoundTripSizeConstrained) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    const SizeRange constraint{1, 20};
    ASSERT_TRUE(write_bit_string(w, content, 13, constraint));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 2> out{};
    auto result = read_bit_string(r, out, constraint);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 13u);
    EXPECT_EQ(out, content);
}

TEST(BitString, FragmentationBoundaryExactly16384Bits) {
    // The distinctive BIT STRING case: fragmentation triggers at 16384 BITS
    // (2048 bytes), not 16384 bytes as it would for OCTET STRING/open_type.
    std::vector<std::byte> content(2048);
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>(i % 256);
    }
    std::vector<std::byte> buf(2100);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_bit_string(w, content, 16384));
    // marker (8) + 16384 content bits + zero-bit terminator (8)
    EXPECT_EQ(w.bits_written(), 8u + 16384u + 8u);

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(2048);
    auto result = read_bit_string(r, out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 16384u);
    EXPECT_EQ(out, content);
}

TEST(BitString, ReaderDestinationTooSmallFails) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    ASSERT_TRUE(write_bit_string(w, content, 13));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 1> out{}; // needs 2 bytes for 13 bits
    auto result = read_bit_string(r, out);
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::BufferExhausted);
}
