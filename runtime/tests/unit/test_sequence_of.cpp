#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/sequence_of.hpp"
#include "v2x/per/size_range.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_sequence_of;
using v2x::per::Result;
using v2x::per::SizeRange;
using v2x::per::Status;
using v2x::per::Unaligned;
using v2x::per::write_sequence_of;

namespace {
// Element type for these tests: an 8-bit-fixed-width INTEGER (0..255).
Status<> EncodeByteElement(PerWriter<Unaligned>& w, uint32_t index, const std::vector<int64_t>& values) {
    return w.constrained_whole_number(values[index], 0, 255);
}
} // namespace

TEST(SequenceOf, ZeroElementsKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    bool called = false;
    auto status = write_sequence_of(w, 0, std::nullopt,
                                     [&](auto&, uint32_t) -> Status<> {
                                         called = true;
                                         return Status<>::Ok();
                                     });
    ASSERT_TRUE(status.ok());
    EXPECT_FALSE(called);
    EXPECT_EQ(w.bits_written(), 8u);
    EXPECT_EQ(buf[0], std::byte{0x00});
}

TEST(SequenceOf, OneElementKnownAnswer) {
    std::array<std::byte, 3> buf{};
    PerWriter<Unaligned> w(buf);
    const std::vector<int64_t> values{42};
    auto status = write_sequence_of(w, 1, std::nullopt,
                                     [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); });
    ASSERT_TRUE(status.ok());
    EXPECT_EQ(w.bits_written(), 16u); // 8 (length) + 8 (one element)
    EXPECT_EQ(buf[0], std::byte{0x01});
    EXPECT_EQ(buf[1], std::byte{42});
}

TEST(SequenceOf, RoundTripManyElementsNoFragmentation) {
    std::vector<int64_t> values;
    for (int i = 0; i < 100; ++i) {
        values.push_back((i * 3) % 256);
    }
    std::vector<std::byte> buf(200);
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_sequence_of(w, static_cast<uint32_t>(values.size()), std::nullopt,
                                   [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); }));

    PerReader<Unaligned> r(buf);
    std::vector<int64_t> decoded;
    auto result = read_sequence_of(r, std::nullopt, [&](auto& inner, uint32_t) -> Status<> {
        auto v = inner.constrained_whole_number(0, 255);
        if (!v) {
            return Status<>::Err(v.error());
        }
        decoded.push_back(v.value());
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 100u);
    EXPECT_EQ(decoded, values);
}

TEST(SequenceOf, SizeConstrainedDelegationKnownAnswer) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::vector<int64_t> values{1, 2, 3, 4, 5};
    const SizeRange constraint{0, 10}; // span=10, bit_width=4
    auto status = write_sequence_of(w, 5, constraint,
                                     [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); });
    ASSERT_TRUE(status.ok());
    EXPECT_EQ(w.bits_written(), 4u + 5u * 8u);
}

TEST(SequenceOf, FragmentationBoundary16384ElementsRoundTrip) {
    constexpr uint32_t kCount = 16384;
    std::vector<std::byte> buf(kCount + 100);
    PerWriter<Unaligned> w(buf);
    auto status = write_sequence_of(w, kCount, std::nullopt, [](auto& inner, uint32_t idx) -> Status<> {
        return inner.constrained_whole_number(static_cast<int64_t>(idx % 256), 0, 255);
    });
    ASSERT_TRUE(status.ok());
    // marker (8) + 16384 elements * 8 bits + zero-element terminator (8)
    EXPECT_EQ(w.bits_written(), 8u + kCount * 8u + 8u);

    PerReader<Unaligned> r(buf);
    uint32_t mismatches = 0;
    auto result = read_sequence_of(r, std::nullopt, [&](auto& inner, uint32_t idx) -> Status<> {
        auto v = inner.constrained_whole_number(0, 255);
        if (!v) {
            return Status<>::Err(v.error());
        }
        if (v.value() != static_cast<int64_t>(idx % 256)) {
            ++mismatches;
        }
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), kCount);
    EXPECT_EQ(mismatches, 0u);
}

TEST(SequenceOf, DecodeCallbackBoundsCheckErrorPropagates) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::vector<int64_t> values{1, 2, 3};
    ASSERT_TRUE(write_sequence_of(w, 3, std::nullopt,
                                   [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); }));

    PerReader<Unaligned> r(buf);
    constexpr uint32_t kCapacity = 2; // deliberately smaller than the encoded count
    auto result = read_sequence_of(r, std::nullopt, [&](auto& inner, uint32_t idx) -> Status<> {
        if (idx >= kCapacity) {
            return Status<>::Err(Error::BufferExhausted);
        }
        auto v = inner.constrained_whole_number(0, 255);
        if (!v) {
            return Status<>::Err(v.error());
        }
        return Status<>::Ok();
    });
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::BufferExhausted);
}

TEST(SequenceOf, WriteBufferExhaustedPropagatesFromElementEncode) {
    std::array<std::byte, 1> buf{}; // room for the length byte only
    PerWriter<Unaligned> w(buf);
    const std::vector<int64_t> values{1};
    auto status = write_sequence_of(w, 1, std::nullopt,
                                     [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); });
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::BufferExhausted);
}
