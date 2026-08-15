#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/sequence_of.hpp"
#include "v2x/per/size_range.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

using v2x::per::Error;
using v2x::per::SizeRange;
using v2x::per::Status;
using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::read_extensible_sequence_of;
using v2x::per::read_sequence_of;
using v2x::per::write_extensible_sequence_of;
using v2x::per::write_sequence_of;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 800;
constexpr int kBoundaryIterations = 30;

// Elements are constrained_whole_number(0, 255) — the element shape itself is
// already covered by test_property_integers.cpp; this file's job is the
// count/length-determinant/fragmentation machinery around a sequence of them.
std::vector<int64_t> RandomElements(std::mt19937_64& rng, uint32_t count) {
    std::vector<int64_t> elements(count);
    for (auto& e : elements) {
        e = RandomInRange(rng, 0, 255);
    }
    return elements;
}
} // namespace

TEST(PropertySequenceOf, RoundTripNoConstraint) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t count = static_cast<uint32_t>(RandomInRange(rng, 0, 200));
        const auto elements = RandomElements(rng, count);

        std::array<std::byte, 4096> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_sequence_of(w, count, std::nullopt, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(elements[idx], 0, 255);
        })) << "count=" << count;

        std::vector<int64_t> decoded(count);
        UperReader r(buf);
        auto decoded_count = read_sequence_of(r, std::nullopt, [&](auto& reader, uint32_t idx) -> Status<> {
            auto v = reader.constrained_whole_number(0, 255);
            if (!v) {
                return Status<>::Err(v.error());
            }
            if (idx >= decoded.size()) {
                return Status<>::Err(Error::BufferExhausted);
            }
            decoded[idx] = v.value();
            return Status<>::Ok();
        });
        ASSERT_TRUE(decoded_count.ok()) << "count=" << count;
        ASSERT_EQ(decoded_count.value(), count) << "count=" << count;
        EXPECT_EQ(decoded, elements) << "count=" << count;
    }
}

TEST(PropertySequenceOf, RoundTripWithSizeConstraint) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t min = static_cast<uint32_t>(RandomInRange(rng, 0, 10));
        const uint32_t max = min + static_cast<uint32_t>(RandomInRange(rng, 0, 100));
        const SizeRange constraint{min, max};
        const uint32_t count = static_cast<uint32_t>(RandomInRange(rng, min, max));
        const auto elements = RandomElements(rng, count);

        std::array<std::byte, 4096> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_sequence_of(w, count, constraint, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(elements[idx], 0, 255);
        })) << "count=" << count;

        std::vector<int64_t> decoded(count);
        UperReader r(buf);
        auto decoded_count = read_sequence_of(r, constraint, [&](auto& reader, uint32_t idx) -> Status<> {
            auto v = reader.constrained_whole_number(0, 255);
            if (!v) {
                return Status<>::Err(v.error());
            }
            if (idx >= decoded.size()) {
                return Status<>::Err(Error::BufferExhausted);
            }
            decoded[idx] = v.value();
            return Status<>::Ok();
        });
        ASSERT_TRUE(decoded_count.ok()) << "count=" << count;
        ASSERT_EQ(decoded_count.value(), count) << "count=" << count;
        EXPECT_EQ(decoded, elements) << "count=" << count;
    }
}

TEST(PropertySequenceOf, ExtensibleRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t min = static_cast<uint32_t>(RandomInRange(rng, 0, 10));
        const uint32_t max = min + static_cast<uint32_t>(RandomInRange(rng, 0, 30));
        const SizeRange root{min, max};
        const bool in_root = RandomInRange(rng, 0, 1) == 0;
        const uint32_t count = in_root ? static_cast<uint32_t>(RandomInRange(rng, min, max))
                                        : max + 1 + static_cast<uint32_t>(RandomInRange(rng, 0, 50));
        const auto elements = RandomElements(rng, count);

        std::array<std::byte, 4096> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_extensible_sequence_of(w, count, root, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(elements[idx], 0, 255);
        })) << "count=" << count << " in_root=" << in_root;

        std::vector<int64_t> decoded(count);
        UperReader r(buf);
        auto decoded_count = read_extensible_sequence_of(r, root, [&](auto& reader, uint32_t idx) -> Status<> {
            auto v = reader.constrained_whole_number(0, 255);
            if (!v) {
                return Status<>::Err(v.error());
            }
            if (idx >= decoded.size()) {
                return Status<>::Err(Error::BufferExhausted);
            }
            decoded[idx] = v.value();
            return Status<>::Ok();
        });
        ASSERT_TRUE(decoded_count.ok()) << "count=" << count << " in_root=" << in_root;
        ASSERT_EQ(decoded_count.value(), count) << "count=" << count << " in_root=" << in_root;
        EXPECT_EQ(decoded, elements) << "count=" << count << " in_root=" << in_root;
    }
}

// Fewer iterations, element counts biased around the 16384-element fragmentation
// boundary — the small-count tests above almost never reach it.
TEST(PropertySequenceOf, RoundTripNearFragmentationBoundary) {
    auto& rng = Rng();
    for (int i = 0; i < kBoundaryIterations; ++i) {
        const uint32_t count = static_cast<uint32_t>(RandomInRange(rng, 16184, 16584));
        const auto elements = RandomElements(rng, count);

        std::vector<std::byte> buf(static_cast<size_t>(count) + 256);
        UperWriter w(buf);
        ASSERT_TRUE(write_sequence_of(w, count, std::nullopt, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(elements[idx], 0, 255);
        })) << "count=" << count;

        std::vector<int64_t> decoded(count);
        UperReader r(buf);
        auto decoded_count = read_sequence_of(r, std::nullopt, [&](auto& reader, uint32_t idx) -> Status<> {
            auto v = reader.constrained_whole_number(0, 255);
            if (!v) {
                return Status<>::Err(v.error());
            }
            if (idx >= decoded.size()) {
                return Status<>::Err(Error::BufferExhausted);
            }
            decoded[idx] = v.value();
            return Status<>::Ok();
        });
        ASSERT_TRUE(decoded_count.ok()) << "count=" << count;
        ASSERT_EQ(decoded_count.value(), count) << "count=" << count;
        EXPECT_EQ(decoded, elements) << "count=" << count;
    }
}
