#include <gtest/gtest.h>

#include <cstdint>
#include <utility>

#include "v2x/per/error.hpp"
#include "v2x/per/result.hpp"

using v2x::per::Error;
using v2x::per::Result;
using v2x::per::Status;

TEST(Result, OkHoldsValue) {
    auto r = Result<int64_t>::Ok(42);
    ASSERT_TRUE(r.ok());
    ASSERT_TRUE(static_cast<bool>(r));
    EXPECT_EQ(r.value(), 42);
}

TEST(Result, ErrHoldsError) {
    auto r = Result<int64_t>::Err(Error::ValueOutOfRange);
    ASSERT_FALSE(r.ok());
    EXPECT_EQ(r.error(), Error::ValueOutOfRange);
}

TEST(Result, CopyPreservesOkState) {
    auto r = Result<int64_t>::Ok(7);
    auto copy = r; // NOLINT
    EXPECT_TRUE(copy.ok());
    EXPECT_EQ(copy.value(), 7);
}

TEST(Result, CopyPreservesErrState) {
    auto r = Result<int64_t>::Err(Error::BufferExhausted);
    auto copy = r; // NOLINT
    EXPECT_FALSE(copy.ok());
    EXPECT_EQ(copy.error(), Error::BufferExhausted);
}

TEST(Result, MoveExtractsValue) {
    auto r = Result<int64_t>::Ok(99);
    int64_t v = std::move(r).value();
    EXPECT_EQ(v, 99);
}

TEST(Result, AssignmentSwitchesState) {
    auto r = Result<int64_t>::Ok(1);
    r = Result<int64_t>::Err(Error::LengthTooLarge);
    EXPECT_FALSE(r.ok());
    EXPECT_EQ(r.error(), Error::LengthTooLarge);
}

TEST(Status, OkHasNoError) {
    auto s = Status<>::Ok();
    EXPECT_TRUE(s.ok());
    EXPECT_TRUE(static_cast<bool>(s));
}

TEST(Status, ErrHoldsError) {
    auto s = Status<>::Err(Error::MalformedEncoding);
    EXPECT_FALSE(s.ok());
    EXPECT_EQ(s.error(), Error::MalformedEncoding);
}
