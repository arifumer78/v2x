#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include "v2x/per/choice.hpp"
#include "v2x/per/enumerated.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::read_choice_selector;
using v2x::per::read_enumerated;
using v2x::per::read_open_type_wrapped;
using v2x::per::write_choice_extension;
using v2x::per::write_choice_root_index;
using v2x::per::write_enumerated_extension;
using v2x::per::write_enumerated_root;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 2000;
}

TEST(PropertyEnumerated, RootValueRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n_root = static_cast<uint32_t>(RandomInRange(rng, 1, 500));
        const uint32_t index = static_cast<uint32_t>(RandomInRange(rng, 0, n_root - 1));
        const bool extensible = RandomInRange(rng, 0, 1) != 0;

        std::array<std::byte, 16> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_enumerated_root(w, index, n_root, extensible)) << "n_root=" << n_root << " index=" << index;

        UperReader r(buf);
        auto decoded = read_enumerated(r, n_root, extensible);
        ASSERT_TRUE(decoded.ok()) << "n_root=" << n_root << " index=" << index;
        EXPECT_FALSE(decoded.value().is_extension);
        EXPECT_EQ(decoded.value().index, index) << "n_root=" << n_root << " index=" << index;
    }
}

TEST(PropertyEnumerated, ExtensionValueRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        // n_root_values only matters for the (untaken, is_extension=true) root branch of
        // read_enumerated — any positive value is fine here, kept fixed for clarity.
        const uint32_t n_root = 10;
        const uint32_t ext_index = RandomInRange(rng, 0, 1) == 0 ? static_cast<uint32_t>(RandomInRange(rng, 0, 63))
                                                                   : static_cast<uint32_t>(RandomInRange(rng, 64, 1'000'000));

        std::array<std::byte, 16> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_enumerated_extension(w, ext_index)) << "ext_index=" << ext_index;

        UperReader r(buf);
        auto decoded = read_enumerated(r, n_root, /*extensible=*/true);
        ASSERT_TRUE(decoded.ok()) << "ext_index=" << ext_index;
        EXPECT_TRUE(decoded.value().is_extension);
        EXPECT_EQ(decoded.value().index, ext_index) << "ext_index=" << ext_index;
    }
}

TEST(PropertyChoice, RootIndexRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n_alts = static_cast<uint32_t>(RandomInRange(rng, 1, 500));
        const uint32_t index = static_cast<uint32_t>(RandomInRange(rng, 0, n_alts - 1));

        std::array<std::byte, 16> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_choice_root_index(w, index, n_alts)) << "n_alts=" << n_alts << " index=" << index;

        UperReader r(buf);
        auto decoded = read_choice_selector(r, n_alts);
        ASSERT_TRUE(decoded.ok()) << "n_alts=" << n_alts << " index=" << index;
        EXPECT_FALSE(decoded.value().is_extension);
        EXPECT_EQ(decoded.value().index, index) << "n_alts=" << n_alts << " index=" << index;
    }
}

TEST(PropertyChoice, ExtensionAlternativeRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n_alts = 5; // only matters for the untaken root branch, see PropertyEnumerated note above
        const uint32_t ext_index = RandomInRange(rng, 0, 1) == 0 ? static_cast<uint32_t>(RandomInRange(rng, 0, 63))
                                                                   : static_cast<uint32_t>(RandomInRange(rng, 64, 10'000));
        const int64_t payload = RandomInRange(rng, 0, 1000);

        std::array<std::byte, 64> buf{};
        UperWriter w(buf);
        std::array<std::byte, 32> scratch{};
        ASSERT_TRUE(write_choice_extension(w, ext_index, scratch, [&](auto& inner) {
            return inner.constrained_whole_number(payload, 0, 1000);
        })) << "ext_index=" << ext_index << " payload=" << payload;

        UperReader r(buf);
        auto selector = read_choice_selector(r, n_alts);
        ASSERT_TRUE(selector.ok()) << "ext_index=" << ext_index;
        EXPECT_TRUE(selector.value().is_extension);
        EXPECT_EQ(selector.value().index, ext_index) << "ext_index=" << ext_index;

        std::array<std::byte, 32> unwrap_scratch{};
        auto decoded_payload =
            read_open_type_wrapped(r, unwrap_scratch, [&](auto& inner) { return inner.constrained_whole_number(0, 1000); });
        ASSERT_TRUE(decoded_payload.ok()) << "ext_index=" << ext_index << " payload=" << payload;
        EXPECT_EQ(decoded_payload.value(), payload) << "ext_index=" << ext_index << " payload=" << payload;
    }
}
