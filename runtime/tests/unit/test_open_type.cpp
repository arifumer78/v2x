#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;

TEST(OpenType, EmptyContentKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.open_type(std::span<const std::byte>{}));
    EXPECT_EQ(w.bits_written(), 8u); // just the length byte, n=0
    EXPECT_EQ(buf[0], std::byte{0x00});

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 1> out{};
    auto result = r.open_type(out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 0u);
}

TEST(OpenType, SingleByteRoundTrip) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 1> content{std::byte{0xAB}};
    ASSERT_TRUE(w.open_type(content));
    EXPECT_EQ(w.bits_written(), 16u); // 8 (length) + 8 (content)

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 1> out{};
    auto result = r.open_type(out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 1u);
    EXPECT_EQ(out[0], std::byte{0xAB});
}

TEST(OpenType, MediumSizeRoundTripNoFragmentation) {
    std::vector<std::byte> content(5000);
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>(i % 256);
    }
    std::vector<std::byte> buf(5100);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.open_type(content));

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(5000);
    auto result = r.open_type(out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 5000u);
    EXPECT_EQ(out, content);
}

TEST(OpenType, ExactFragmentBoundary16384RoundTrip) {
    std::vector<std::byte> content(16384);
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>(i % 256);
    }
    std::vector<std::byte> buf(16400);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.open_type(content));
    // marker (8) + 16384 content bytes (131072) + zero-length terminator (8)
    EXPECT_EQ(w.bits_written(), 8u + 16384u * 8u + 8u);

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(16384);
    auto result = r.open_type(out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 16384u);
    EXPECT_EQ(out, content);
}

TEST(OpenType, FragmentedContentBeyondOneChunkRoundTrip) {
    std::vector<std::byte> content(40000); // > 2*16384, needs 3 length_determinant calls
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>((i * 7) % 256);
    }
    std::vector<std::byte> buf(40100);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.open_type(content));

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(40000);
    auto result = r.open_type(out);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 40000u);
    EXPECT_EQ(out, content);
}

TEST(OpenType, ReaderDestinationTooSmallFails) {
    std::array<std::byte, 16> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 10> content{};
    ASSERT_TRUE(w.open_type(content));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 5> out{}; // too small for 10 bytes of content
    auto result = r.open_type(out);
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::BufferExhausted);
}

TEST(OpenType, WriterBufferExhausted) {
    std::array<std::byte, 1> buf{}; // room for the length byte only
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 5> content{};
    auto status = w.open_type(content);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}
