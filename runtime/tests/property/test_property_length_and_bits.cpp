#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "v2x/per/size_range.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

using v2x::per::SizeRange;
using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::test::RandomBytes;
using v2x::per::test::RandomCountNearBoundary;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 3000;
}

// length_determinant only ever commits to a single header per call (see
// per_writer.hpp's comment) — for n < 16384 that header covers all of n, so the
// round-trip property is "decode == n"; at/above 16384 it covers a chunk (a multiple
// of 16384, capped at 4), so the property is "decode == the chunk write returned",
// not "decode == n". Both are exercised here, biased near the boundary.
TEST(PropertyLengthDeterminant, UnconstrainedRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        uint32_t n = 0;
        switch (RandomInRange(rng, 0, 2)) {
            case 0:
                n = static_cast<uint32_t>(RandomInRange(rng, 0, 127)); // single-byte short form
                break;
            case 1:
                n = static_cast<uint32_t>(RandomInRange(rng, 128, 16383)); // two-byte medium form
                break;
            default:
                n = RandomCountNearBoundary(rng); // fragment-marker form, biased at 16384
                break;
        }

        std::array<std::byte, 8> buf{};
        UperWriter w(buf);
        auto written = w.length_determinant(n, std::nullopt);
        ASSERT_TRUE(written.ok()) << "n=" << n;

        UperReader r(buf);
        auto decoded = r.length_determinant(std::nullopt);
        ASSERT_TRUE(decoded.ok()) << "n=" << n;
        EXPECT_EQ(decoded.value(), written.value()) << "n=" << n;
    }
}

// With a constraint whose max fits in <=65535, length_determinant fully delegates to
// constrained_whole_number — no fragmentation possible, so decode == n exactly.
TEST(PropertyLengthDeterminant, ConstrainedRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t min = static_cast<uint32_t>(RandomInRange(rng, 0, 1000));
        const uint32_t max = min + static_cast<uint32_t>(RandomInRange(rng, 0, 60000));
        const SizeRange constraint{min, max};
        const uint32_t n = static_cast<uint32_t>(RandomInRange(rng, min, max));

        std::array<std::byte, 8> buf{};
        UperWriter w(buf);
        auto written = w.length_determinant(n, constraint);
        ASSERT_TRUE(written.ok()) << "min=" << min << " max=" << max << " n=" << n;

        UperReader r(buf);
        auto decoded = r.length_determinant(constraint);
        ASSERT_TRUE(decoded.ok()) << "min=" << min << " max=" << max << " n=" << n;
        EXPECT_EQ(decoded.value(), n) << "min=" << min << " max=" << max << " n=" << n;
    }
}

// bits()/bit() round trip. Restricted to byte-aligned bit counts here — the
// non-byte-aligned/trailing-bits case is exactly BIT STRING's own semantics
// (unspecified trailing bits beyond n_bits), already covered end to end by
// test_property_strings.cpp's BIT STRING coverage rather than duplicated here.
TEST(PropertyBits, ByteAlignedRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const size_t n_bytes = static_cast<size_t>(RandomInRange(rng, 0, 64));
        const auto content = RandomBytes(rng, n_bytes);

        std::array<std::byte, 128> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.bits(content, n_bytes * 8)) << "n_bytes=" << n_bytes;

        std::array<std::byte, 64> out{};
        UperReader r(buf);
        ASSERT_TRUE(r.bits(std::span<std::byte>(out.data(), n_bytes), n_bytes * 8)) << "n_bytes=" << n_bytes;

        for (size_t j = 0; j < n_bytes; ++j) {
            EXPECT_EQ(out[j], content[j]) << "n_bytes=" << n_bytes << " j=" << j;
        }
    }
}

TEST(PropertyBits, SingleBitRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const bool value = RandomInRange(rng, 0, 1) != 0;

        std::array<std::byte, 1> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.bit(value));

        UperReader r(buf);
        auto decoded = r.bit();
        ASSERT_TRUE(decoded.ok());
        EXPECT_EQ(decoded.value(), value);
    }
}
