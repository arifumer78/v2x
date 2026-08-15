#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include "v2x/per/integer.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

// Layer 2 (property-based): decode(encode(x)) == x across randomly generated,
// precondition-respecting inputs, biased toward the bit-width buckets each
// primitive's write/read implementation branches on (see per_writer.hpp comments) —
// broader coverage than Layer 1's hand-picked known-answer/boundary cases, not a
// replacement for them.

using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::read_extensible_integer;
using v2x::per::write_extensible_integer;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 3000;
}

TEST(PropertyIntegers, ConstrainedWholeNumberRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t lb = RandomInRange(rng, -1'000'000'000LL, 1'000'000'000LL);
        int64_t span = 0;
        switch (RandomInRange(rng, 0, 2)) {
            case 0:
                span = 0; // single possible value -> zero bits
                break;
            case 1:
                span = RandomInRange(rng, 1, 65536); // fixed-width path
                break;
            default:
                span = RandomInRange(rng, 65537, 2'000'000'000LL); // length-prefixed fallback
                break;
        }
        const int64_t ub = lb + span;
        const int64_t v = RandomInRange(rng, lb, ub);

        std::array<std::byte, 32> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.constrained_whole_number(v, lb, ub)) << "lb=" << lb << " ub=" << ub << " v=" << v;

        UperReader r(buf);
        auto decoded = r.constrained_whole_number(lb, ub);
        ASSERT_TRUE(decoded.ok()) << "lb=" << lb << " ub=" << ub << " v=" << v;
        EXPECT_EQ(decoded.value(), v) << "lb=" << lb << " ub=" << ub << " v=" << v;
    }
}

TEST(PropertyIntegers, SemiConstrainedWholeNumberRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t lb = RandomInRange(rng, -1'000'000'000LL, 1'000'000'000LL);
        const int64_t offset = RandomInRange(rng, 0, 10'000'000'000LL);
        const int64_t v = lb + offset;

        std::array<std::byte, 32> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.semi_constrained_whole_number(v, lb)) << "lb=" << lb << " v=" << v;

        UperReader r(buf);
        auto decoded = r.semi_constrained_whole_number(lb);
        ASSERT_TRUE(decoded.ok()) << "lb=" << lb << " v=" << v;
        EXPECT_EQ(decoded.value(), v) << "lb=" << lb << " v=" << v;
    }
}

TEST(PropertyIntegers, UnconstrainedWholeNumberRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t v = RandomInRange(rng, INT64_MIN, INT64_MAX);

        std::array<std::byte, 16> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.unconstrained_whole_number(v)) << "v=" << v;

        UperReader r(buf);
        auto decoded = r.unconstrained_whole_number();
        ASSERT_TRUE(decoded.ok()) << "v=" << v;
        EXPECT_EQ(decoded.value(), v) << "v=" << v;
    }
}

TEST(PropertyIntegers, NormallySmallLengthRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n = RandomInRange(rng, 0, 1) == 0 ? static_cast<uint32_t>(RandomInRange(rng, 0, 63))
                                                           : static_cast<uint32_t>(RandomInRange(rng, 64, 1'000'000));

        std::array<std::byte, 16> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(w.normally_small_length(n)) << "n=" << n;

        UperReader r(buf);
        auto decoded = r.normally_small_length();
        ASSERT_TRUE(decoded.ok()) << "n=" << n;
        EXPECT_EQ(decoded.value(), n) << "n=" << n;
    }
}

TEST(PropertyIntegers, ExtensibleIntegerRoundTrip) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t lb = RandomInRange(rng, -1'000'000, 1'000'000);
        const int64_t ub = lb + RandomInRange(rng, 1, 100'000);
        // Half the time stay in-root; half the time force an out-of-root value so both
        // the in-root and unconstrained-fallback branches get exercised.
        const bool in_root = RandomInRange(rng, 0, 1) == 0;
        const int64_t v = in_root ? RandomInRange(rng, lb, ub)
                                   : (RandomInRange(rng, 0, 1) == 0 ? lb - 1 - RandomInRange(rng, 0, 1'000'000)
                                                                     : ub + 1 + RandomInRange(rng, 0, 1'000'000));

        std::array<std::byte, 32> buf{};
        UperWriter w(buf);
        ASSERT_TRUE(write_extensible_integer(w, v, lb, ub)) << "lb=" << lb << " ub=" << ub << " v=" << v;

        UperReader r(buf);
        auto decoded = read_extensible_integer(r, lb, ub);
        ASSERT_TRUE(decoded.ok()) << "lb=" << lb << " ub=" << ub << " v=" << v;
        EXPECT_EQ(decoded.value(), v) << "lb=" << lb << " ub=" << ub << " v=" << v;
    }
}
