#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

#include "v2x/per/error.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_open_type_wrapped;
using v2x::per::Result;
using v2x::per::Status;
using v2x::per::Unaligned;
using v2x::per::write_open_type_wrapped;

TEST(OpenTypeWrap, RoundTripSimpleValue) {
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    auto status = write_open_type_wrapped(w, scratch, [](auto& inner) {
        return inner.constrained_whole_number(42, 0, 1000);
    });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    auto result = read_open_type_wrapped(r, read_scratch,
                                          [](auto& inner) { return inner.constrained_whole_number(0, 1000); });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 42);
}

TEST(OpenTypeWrap, RoundTripMultipleValuesInOneWrap) {
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    auto status = write_open_type_wrapped(w, scratch, [](auto& inner) -> Status<> {
        auto a = inner.constrained_whole_number(5, 0, 15);
        if (!a) return a;
        return inner.constrained_whole_number(200, 0, 255);
    });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    auto result = read_open_type_wrapped(r, read_scratch, [](auto& inner) -> Result<std::pair<int64_t, int64_t>> {
        auto a = inner.constrained_whole_number(0, 15);
        if (!a) return Result<std::pair<int64_t, int64_t>>::Err(a.error());
        auto b = inner.constrained_whole_number(0, 255);
        if (!b) return Result<std::pair<int64_t, int64_t>>::Err(b.error());
        return Result<std::pair<int64_t, int64_t>>::Ok({a.value(), b.value()});
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value().first, 5);
    EXPECT_EQ(result.value().second, 200);
}

TEST(OpenTypeWrap, ScratchTooSmallOnWriteFails) {
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 1> scratch{}; // 8 bits: not enough for a 400-bit value's encoding
    PerWriter<Unaligned> w(buf);
    auto status = write_open_type_wrapped(w, scratch, [](auto& inner) {
        return inner.unconstrained_whole_number(123456789012345LL);
    });
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}

TEST(OpenTypeWrap, EncodeFnErrorPropagatesWithoutTouchingOuterWriter) {
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    auto status = write_open_type_wrapped(w, scratch, [](auto& inner) {
        return inner.constrained_whole_number(2000, 0, 1000); // out of range
    });
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::ValueOutOfRange);
    EXPECT_EQ(w.bits_written(), 0u); // outer writer untouched — open_type was never called
}

TEST(OpenTypeWrap, DecodeFnErrorPropagates) {
    // Hand-write a raw out-of-range pattern (offset=3 for span=2, lb=0,ub=2)
    // — same technique as ConstrainedWholeNumber.DecodeRejectsOffsetBeyondSpan
    // in test_constrained_whole_number.cpp — rather than relying on a
    // bit-width mismatch between encode/decode constraints, which doesn't
    // reliably produce an out-of-range offset.
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_open_type_wrapped(w, scratch, [](auto& inner) -> Status<> {
        auto a = inner.bit(true);
        if (!a) return a;
        return inner.bit(true);
    }));

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    auto result =
        read_open_type_wrapped(r, read_scratch, [](auto& inner) { return inner.constrained_whole_number(0, 2); });
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::MalformedEncoding);
}
