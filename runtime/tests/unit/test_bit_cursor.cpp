#include <gtest/gtest.h>

#include "v2x/per/bit_cursor.hpp"

using v2x::per::BitCursor;

TEST(BitCursor, InitialState) {
    BitCursor c(16);
    EXPECT_EQ(c.position(), 0u);
    EXPECT_EQ(c.total_bits(), 16u);
    EXPECT_EQ(c.remaining_bits(), 16u);
}

TEST(BitCursor, AdvanceWithinBounds) {
    BitCursor c(16);
    EXPECT_TRUE(c.advance(5));
    EXPECT_EQ(c.position(), 5u);
    EXPECT_EQ(c.remaining_bits(), 11u);
}

TEST(BitCursor, AdvanceZeroIsNoOp) {
    BitCursor c(16);
    EXPECT_TRUE(c.advance(0));
    EXPECT_EQ(c.position(), 0u);
}

TEST(BitCursor, AdvanceExactRemaining) {
    BitCursor c(8);
    EXPECT_TRUE(c.advance(8));
    EXPECT_EQ(c.position(), 8u);
    EXPECT_EQ(c.remaining_bits(), 0u);
}

TEST(BitCursor, AdvanceExceedingBoundsFailsAndLeavesPositionUnchanged) {
    BitCursor c(8);
    EXPECT_FALSE(c.advance(9));
    EXPECT_EQ(c.position(), 0u);
    EXPECT_EQ(c.remaining_bits(), 8u);
}

TEST(BitCursor, AdvanceExceedingBoundsAfterPartialAdvanceFails) {
    BitCursor c(8);
    ASSERT_TRUE(c.advance(4));
    EXPECT_FALSE(c.advance(5)); // only 4 bits remain
    EXPECT_EQ(c.position(), 4u);
}

TEST(BitCursor, PadToOctetAlreadyAlignedIsNoOp) {
    BitCursor c(32);
    ASSERT_TRUE(c.advance(8));
    c.pad_to_octet();
    EXPECT_EQ(c.position(), 8u);
}

TEST(BitCursor, PadToOctetAtZeroIsNoOp) {
    BitCursor c(16);
    c.pad_to_octet();
    EXPECT_EQ(c.position(), 0u);
}

TEST(BitCursor, PadToOctetFromMidByte) {
    BitCursor c(32);
    ASSERT_TRUE(c.advance(3));
    c.pad_to_octet();
    EXPECT_EQ(c.position(), 8u);
}

TEST(BitCursor, PadToOctetSaturatesWhenBufferTooShort) {
    BitCursor c(5);
    ASSERT_TRUE(c.advance(3));
    c.pad_to_octet(); // next octet boundary would be 8, but total_bits() is only 5
    EXPECT_EQ(c.position(), 5u);
    EXPECT_EQ(c.remaining_bits(), 0u);
}
