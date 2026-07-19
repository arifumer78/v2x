#include <gtest/gtest.h>

#include <array>
#include <cstddef>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;

TEST(BitPrimitives, WriteSingleBitTrueSetsMSB) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    EXPECT_EQ(buf[0], std::byte{0x80});
}

TEST(BitPrimitives, WriteSingleBitFalseLeavesZero) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(false));
    EXPECT_EQ(buf[0], std::byte{0x00});
}

TEST(BitPrimitives, WriteSequentialBitsPackMSBFirst) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    for (bool bit : {true, false, true, true, false, false, false, false}) {
        ASSERT_TRUE(w.bit(bit));
    }
    EXPECT_EQ(buf[0], std::byte{0b10110000});
}

TEST(BitPrimitives, WriteBitBufferExhausted) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    for (int i = 0; i < 8; ++i) {
        ASSERT_TRUE(w.bit(true));
    }
    auto status = w.bit(true);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}

TEST(BitPrimitives, WriteBitsZeroLengthIsNoOp) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    std::array<std::byte, 1> src{std::byte{0xFF}};
    ASSERT_TRUE(w.bits(src, 0));
    EXPECT_EQ(w.bits_written(), 0u);
}

TEST(BitPrimitives, WriteBitsExceedingBufferFails) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    std::array<std::byte, 2> src{std::byte{0xFF}, std::byte{0xFF}};
    auto status = w.bits(src, 9);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}

TEST(BitPrimitives, WriteThenReadBitsRoundTrip) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    std::array<std::byte, 2> src{std::byte{0b10110101}, std::byte{0b11000000}};
    ASSERT_TRUE(w.bits(src, 10)); // first 10 bits of src: full byte0 + top 2 bits of byte1

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 2> out{};
    ASSERT_TRUE(r.bits(out, 10));
    EXPECT_EQ(out[0], std::byte{0b10110101});
    EXPECT_EQ(out[1], std::byte{0b11000000}); // top 2 bits set, rest zero
}

TEST(BitPrimitives, ReadBitMatchesWrittenSequence) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<bool, 4> pattern{true, false, false, true};
    for (bool bit : pattern) {
        ASSERT_TRUE(w.bit(bit));
    }

    PerReader<Unaligned> r(buf);
    for (bool expected : pattern) {
        auto result = r.bit();
        ASSERT_TRUE(result.ok());
        EXPECT_EQ(result.value(), expected);
    }
}

TEST(BitPrimitives, ReadBitBufferExhausted) {
    std::array<std::byte, 1> buf{std::byte{0xFF}};
    PerReader<Unaligned> r(buf);
    for (int i = 0; i < 8; ++i) {
        ASSERT_TRUE(r.bit());
    }
    auto result = r.bit();
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::BufferExhausted);
}

TEST(BitPrimitives, FinishReturnsCeilingByteCount) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true)); // 3 bits written -> ceil(3/8) = 1 byte
    auto finished = w.finish();
    EXPECT_EQ(finished.size(), 1u);
}

TEST(BitPrimitives, BitsWrittenAndRemainingTrackPosition) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    std::array<std::byte, 1> src{std::byte{0xFF}};
    ASSERT_TRUE(w.bits(src, 5));
    EXPECT_EQ(w.bits_written(), 5u);

    PerReader<Unaligned> r(buf);
    EXPECT_EQ(r.bits_remaining(), 16u);
    std::array<std::byte, 1> out{};
    ASSERT_TRUE(r.bits(out, 5));
    EXPECT_EQ(r.bits_remaining(), 11u);
}
