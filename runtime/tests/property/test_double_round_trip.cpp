#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

#include "v2x/per/choice.hpp"
#include "v2x/per/enumerated.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/octet_string.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/sequence_of.hpp"
#include "v2x/per/uper.hpp"
#include "random_gen.hpp"

// Layer 3 (double round-trip): encode(x) -> bytes1; decode(bytes1) -> x'; encode(x')
// -> bytes2; assert bytes1 == bytes2 byte-for-byte. A distinct property from Layer 2's
// decode(encode(x)) == x — this one catches a decoder that accepts input and a
// re-encoder that doesn't reproduce the exact same wire bytes for it (Layer 2 alone
// never looks at wire bytes after a round trip, only the decoded value).

using v2x::per::Error;
using v2x::per::Status;
using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::read_choice_selector;
using v2x::per::read_enumerated;
using v2x::per::read_octet_string;
using v2x::per::read_sequence_of;
using v2x::per::write_choice_root_index;
using v2x::per::write_enumerated_extension;
using v2x::per::write_enumerated_root;
using v2x::per::write_octet_string;
using v2x::per::write_sequence_of;
using v2x::per::test::RandomBytes;
using v2x::per::test::RandomInRange;
using v2x::per::test::Rng;

namespace {
constexpr int kIterations = 1000;

template <typename Span1, typename Span2>
void ExpectSameBytes(Span1 bytes1, Span2 bytes2) {
    ASSERT_EQ(bytes1.size(), bytes2.size());
    EXPECT_TRUE(std::equal(bytes1.begin(), bytes1.end(), bytes2.begin()));
}
} // namespace

TEST(DoubleRoundTrip, ConstrainedWholeNumber) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t lb = RandomInRange(rng, -1'000'000'000LL, 1'000'000'000LL);
        const int64_t span = RandomInRange(rng, 0, 200'000'000LL);
        const int64_t ub = lb + span;
        const int64_t v = RandomInRange(rng, lb, ub);

        std::array<std::byte, 32> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(w1.constrained_whole_number(v, lb, ub));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = r.constrained_whole_number(lb, ub);
        ASSERT_TRUE(decoded.ok());

        std::array<std::byte, 32> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(w2.constrained_whole_number(decoded.value(), lb, ub));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, SemiConstrainedWholeNumber) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t lb = RandomInRange(rng, -1'000'000'000LL, 1'000'000'000LL);
        const int64_t v = lb + RandomInRange(rng, 0, 10'000'000'000LL);

        std::array<std::byte, 32> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(w1.semi_constrained_whole_number(v, lb));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = r.semi_constrained_whole_number(lb);
        ASSERT_TRUE(decoded.ok());

        std::array<std::byte, 32> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(w2.semi_constrained_whole_number(decoded.value(), lb));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, UnconstrainedWholeNumber) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const int64_t v = RandomInRange(rng, INT64_MIN, INT64_MAX);

        std::array<std::byte, 16> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(w1.unconstrained_whole_number(v));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = r.unconstrained_whole_number();
        ASSERT_TRUE(decoded.ok());

        std::array<std::byte, 16> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(w2.unconstrained_whole_number(decoded.value()));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, OctetString) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const size_t n = static_cast<size_t>(RandomInRange(rng, 0, 300));
        const auto content = RandomBytes(rng, n);

        std::array<std::byte, 512> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(write_octet_string(w1, content));
        auto bytes1 = w1.finish();

        std::vector<std::byte> out(n);
        UperReader r(buf1);
        auto decoded = read_octet_string(r, out);
        ASSERT_TRUE(decoded.ok());

        std::array<std::byte, 512> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(write_octet_string(w2, out));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, SequenceOf) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t count = static_cast<uint32_t>(RandomInRange(rng, 0, 200));
        std::vector<int64_t> elements(count);
        for (auto& e : elements) {
            e = RandomInRange(rng, 0, 255);
        }

        std::array<std::byte, 4096> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(write_sequence_of(w1, count, std::nullopt, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(elements[idx], 0, 255);
        }));
        auto bytes1 = w1.finish();

        std::vector<int64_t> decoded(count);
        UperReader r(buf1);
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
        ASSERT_TRUE(decoded_count.ok());

        std::array<std::byte, 4096> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(write_sequence_of(w2, decoded_count.value(), std::nullopt, [&](auto& writer, uint32_t idx) {
            return writer.constrained_whole_number(decoded[idx], 0, 255);
        }));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, EnumeratedRootValue) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n_root = static_cast<uint32_t>(RandomInRange(rng, 1, 500));
        const uint32_t index = static_cast<uint32_t>(RandomInRange(rng, 0, n_root - 1));

        std::array<std::byte, 16> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(write_enumerated_root(w1, index, n_root, /*extensible=*/true));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = read_enumerated(r, n_root, /*extensible=*/true);
        ASSERT_TRUE(decoded.ok());
        ASSERT_FALSE(decoded.value().is_extension);

        std::array<std::byte, 16> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(write_enumerated_root(w2, decoded.value().index, n_root, /*extensible=*/true));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, EnumeratedExtensionValue) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t ext_index = RandomInRange(rng, 0, 1) == 0 ? static_cast<uint32_t>(RandomInRange(rng, 0, 63))
                                                                   : static_cast<uint32_t>(RandomInRange(rng, 64, 100'000));

        std::array<std::byte, 16> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(write_enumerated_extension(w1, ext_index));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = read_enumerated(r, /*n_root_values=*/10, /*extensible=*/true);
        ASSERT_TRUE(decoded.ok());
        ASSERT_TRUE(decoded.value().is_extension);

        std::array<std::byte, 16> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(write_enumerated_extension(w2, decoded.value().index));
        ExpectSameBytes(bytes1, w2.finish());
    }
}

TEST(DoubleRoundTrip, ChoiceRootIndex) {
    auto& rng = Rng();
    for (int i = 0; i < kIterations; ++i) {
        const uint32_t n_alts = static_cast<uint32_t>(RandomInRange(rng, 1, 500));
        const uint32_t index = static_cast<uint32_t>(RandomInRange(rng, 0, n_alts - 1));

        std::array<std::byte, 16> buf1{};
        UperWriter w1(buf1);
        ASSERT_TRUE(write_choice_root_index(w1, index, n_alts));
        auto bytes1 = w1.finish();

        UperReader r(buf1);
        auto decoded = read_choice_selector(r, n_alts);
        ASSERT_TRUE(decoded.ok());
        ASSERT_FALSE(decoded.value().is_extension);

        std::array<std::byte, 16> buf2{};
        UperWriter w2(buf2);
        ASSERT_TRUE(write_choice_root_index(w2, decoded.value().index, n_alts));
        ExpectSameBytes(bytes1, w2.finish());
    }
}
