#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "v2x/per/bit_string.hpp"
#include "v2x/per/octet_string.hpp"
#include "v2x/per/size_range.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

using v2x::per::SizeRange;
using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::read_bit_string;
using v2x::per::read_extensible_bit_string;
using v2x::per::read_extensible_octet_string;
using v2x::per::read_octet_string;
using v2x::per::write_bit_string;
using v2x::per::write_extensible_bit_string;
using v2x::per::write_extensible_octet_string;
using v2x::per::write_octet_string;
using v2x::per::test::RandomBytes;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 1500;
constexpr int kBoundaryIterations = 60;

// bit 0 is the MSB of byte 0 (PER convention, see bit_ops.hpp) — used to compare
// only the meaningful n_bits of a BIT STRING, ignoring the unspecified trailing bits
// of a non-byte-aligned final byte.
bool GetBit(std::span<const std::byte> buf, size_t bit_index) {
    const size_t byte_index = bit_index / 8;
    const size_t bit_in_byte = bit_index % 8;
    const auto byte = static_cast<uint8_t>(buf[byte_index]);
    return ((byte >> (7 - bit_in_byte)) & 1u) != 0;
}
} // namespace

TEST(PropertyOctetString, RoundTripNoConstraint) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const size_t n = static_cast<size_t>(RandomInRange(rng, 0, 300));
        const auto content = RandomBytes(rng, n);

        std::array<std::byte, 512> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_octet_string(w, content)) << "n=" << n;

        std::array<std::byte, 300> out{};
        UperReader r(buf);
        auto decoded = read_octet_string(r, out);
        ASSERT_TRUE(decoded.ok()) << "n=" << n;
        ASSERT_EQ(decoded.value(), n) << "n=" << n;
        for (size_t j = 0; j < n; ++j) {
            EXPECT_EQ(out[j], content[j]) << "n=" << n << " j=" << j;
        }
    }
}

TEST(PropertyOctetString, RoundTripWithSizeConstraint) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const size_t min = static_cast<size_t>(RandomInRange(rng, 0, 20));
        const size_t max = min + static_cast<size_t>(RandomInRange(rng, 0, 200));
        const SizeRange constraint{static_cast<uint32_t>(min), static_cast<uint32_t>(max)};
        const size_t n = static_cast<size_t>(RandomInRange(rng, static_cast<int64_t>(min), static_cast<int64_t>(max)));
        const auto content = RandomBytes(rng, n);

        std::array<std::byte, 512> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_octet_string(w, content, constraint)) << "n=" << n;

        std::array<std::byte, 220> out{};
        UperReader r(buf);
        auto decoded = read_octet_string(r, out, constraint);
        ASSERT_TRUE(decoded.ok()) << "n=" << n;
        ASSERT_EQ(decoded.value(), n) << "n=" << n;
        for (size_t j = 0; j < n; ++j) {
            EXPECT_EQ(out[j], content[j]) << "n=" << n << " j=" << j;
        }
    }
}

// Randomly picks values both inside and outside the extension root, exercising both
// write_extensible_octet_string branches (in-root delegates with the constraint;
// out-of-root delegates unconstrained).
TEST(PropertyOctetString, ExtensibleRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t min = static_cast<uint32_t>(RandomInRange(rng, 0, 20));
        const uint32_t max = min + static_cast<uint32_t>(RandomInRange(rng, 0, 50));
        const SizeRange root{min, max};
        const bool in_root = RandomInRange(rng, 0, 1) == 0;
        const size_t n = in_root ? static_cast<size_t>(RandomInRange(rng, min, max))
                                  : static_cast<size_t>(max) + 1 + static_cast<size_t>(RandomInRange(rng, 0, 100));
        const auto content = RandomBytes(rng, n);

        std::array<std::byte, 512> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_extensible_octet_string(w, content, root)) << "n=" << n << " in_root=" << in_root;

        std::array<std::byte, 200> out{};
        UperReader r(buf);
        auto decoded = read_extensible_octet_string(r, out, root);
        ASSERT_TRUE(decoded.ok()) << "n=" << n << " in_root=" << in_root;
        ASSERT_EQ(decoded.value(), n) << "n=" << n << " in_root=" << in_root;
        for (size_t j = 0; j < n; ++j) {
            EXPECT_EQ(out[j], content[j]) << "n=" << n << " j=" << j;
        }
    }
}

// Fewer iterations, deliberately large content — stresses the 16384-byte
// fragmentation boundary that the small-content tests above almost never reach.
TEST(PropertyOctetString, RoundTripNearFragmentationBoundary) {
    auto& rng = Rng();
    for (int i = 0; i < kBoundaryIterations; ++i) {
        const size_t n = static_cast<size_t>(RandomInRange(rng, 16184, 16584));
        const auto content = RandomBytes(rng, n);

        std::vector<std::byte> buf(n + 64);
        UperWriter w(buf);
        ASSERT_TRUE(write_octet_string(w, content)) << "n=" << n;

        std::vector<std::byte> out(n);
        UperReader r(buf);
        auto decoded = read_octet_string(r, out);
        ASSERT_TRUE(decoded.ok()) << "n=" << n;
        ASSERT_EQ(decoded.value(), n) << "n=" << n;
        EXPECT_TRUE(std::equal(out.begin(), out.end(), content.begin())) << "n=" << n;
    }
}

TEST(PropertyBitString, RoundTripNoConstraint) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const size_t n_bits = static_cast<size_t>(RandomInRange(rng, 0, 500));
        const size_t n_bytes = (n_bits + 7) / 8;
        const auto content = RandomBytes(rng, n_bytes);

        std::array<std::byte, 512> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_bit_string(w, content, n_bits)) << "n_bits=" << n_bits;

        std::array<std::byte, 64> out{};
        UperReader r(buf);
        auto decoded = read_bit_string(r, out, std::nullopt);
        ASSERT_TRUE(decoded.ok()) << "n_bits=" << n_bits;
        ASSERT_EQ(decoded.value(), n_bits) << "n_bits=" << n_bits;
        for (size_t bit = 0; bit < n_bits; ++bit) {
            EXPECT_EQ(GetBit(out, bit), GetBit(content, bit)) << "n_bits=" << n_bits << " bit=" << bit;
        }
    }
}

TEST(PropertyBitString, ExtensibleRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t min = static_cast<uint32_t>(RandomInRange(rng, 0, 20));
        const uint32_t max = min + static_cast<uint32_t>(RandomInRange(rng, 0, 400));
        const SizeRange root{min, max};
        const bool in_root = RandomInRange(rng, 0, 1) == 0;
        const size_t n_bits = in_root ? static_cast<size_t>(RandomInRange(rng, min, max))
                                       : static_cast<size_t>(max) + 1 + static_cast<size_t>(RandomInRange(rng, 0, 100));
        const size_t n_bytes = (n_bits + 7) / 8;
        const auto content = RandomBytes(rng, n_bytes);

        std::array<std::byte, 512> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_extensible_bit_string(w, content, n_bits, root)) << "n_bits=" << n_bits;

        std::array<std::byte, 64> out{};
        UperReader r(buf);
        auto decoded = read_extensible_bit_string(r, out, root);
        ASSERT_TRUE(decoded.ok()) << "n_bits=" << n_bits;
        ASSERT_EQ(decoded.value(), n_bits) << "n_bits=" << n_bits;
        for (size_t bit = 0; bit < n_bits; ++bit) {
            EXPECT_EQ(GetBit(out, bit), GetBit(content, bit)) << "n_bits=" << n_bits << " bit=" << bit;
        }
    }
}
