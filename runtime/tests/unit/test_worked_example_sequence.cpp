#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

#include "v2x/per/error.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/sequence_extension.hpp"
#include "v2x/per/uper.hpp"

// The worked example from the implementation plan: a hand-written (not
// generated) SEQUENCE with 2 mandatory INTEGER fields, 1 optional BOOLEAN,
// and extensibility with 1 addition-group INTEGER field. Proof the design
// composes usably end to end, not just in isolated per-construct tests.
//
// TestSeq ::= SEQUENCE {
//     a INTEGER (0..100),
//     b INTEGER (0..1000),
//     c BOOLEAN OPTIONAL,
//     ...,
//     d INTEGER (0..10) OPTIONAL   -- extension addition slot 0
// }

using v2x::per::Error;
using v2x::per::read_sequence_extension;
using v2x::per::Result;
using v2x::per::Status;
using v2x::per::UperReader;
using v2x::per::UperWriter;
using v2x::per::write_sequence_extension;

namespace {

struct TestSeq {
    int64_t a = 0;
    int64_t b = 0;
    std::optional<bool> c;
    std::optional<int64_t> d;
};

Status<> EncodeTestSeq(UperWriter& w, const TestSeq& v) {
    // Extension bit: true iff any addition-group field is present. With only
    // one addition slot (d) this collapses to d.has_value(); a type with
    // more addition slots would OR them all together.
    auto eb = w.bit(v.d.has_value());
    if (!eb) {
        return eb;
    }
    auto cb = w.bit(v.c.has_value()); // preamble bitmap — root optionals only, not d
    if (!cb) {
        return cb;
    }
    if (auto s = w.constrained_whole_number(v.a, 0, 100); !s) {
        return s;
    }
    if (auto s = w.constrained_whole_number(v.b, 0, 1000); !s) {
        return s;
    }
    if (v.c) {
        if (auto s = w.bit(*v.c); !s) {
            return s;
        }
    }
    if (v.d) {
        const std::array<bool, 1> presence{true};
        std::array<std::byte, 16> scratch{};
        return write_sequence_extension(w, presence, scratch, [&](auto& inner, size_t idx) -> Status<> {
            if (idx == 0) {
                return inner.constrained_whole_number(*v.d, 0, 10);
            }
            return Status<>::Err(Error::InvalidConstraint); // unreachable for this type
        });
    }
    return Status<>::Ok();
}

Result<TestSeq> DecodeTestSeq(UperReader& r) {
    auto eb = r.bit();
    if (!eb) {
        return Result<TestSeq>::Err(eb.error());
    }
    auto cb = r.bit();
    if (!cb) {
        return Result<TestSeq>::Err(cb.error());
    }

    TestSeq v;
    auto a = r.constrained_whole_number(0, 100);
    if (!a) {
        return Result<TestSeq>::Err(a.error());
    }
    v.a = a.value();

    auto b = r.constrained_whole_number(0, 1000);
    if (!b) {
        return Result<TestSeq>::Err(b.error());
    }
    v.b = b.value();

    if (cb.value()) {
        auto c = r.bit();
        if (!c) {
            return Result<TestSeq>::Err(c.error());
        }
        v.c = c.value();
    }

    if (eb.value()) {
        std::array<std::byte, 16> scratch{};
        auto ext_result = read_sequence_extension(r, /*known_count=*/1, scratch, [&](auto& inner, uint32_t idx) -> Status<> {
            if (idx == 0) {
                auto d = inner.constrained_whole_number(0, 10);
                if (!d) {
                    return Status<>::Err(d.error());
                }
                v.d = d.value();
                return Status<>::Ok();
            }
            return Status<>::Err(Error::InvalidConstraint); // unreachable for this type
        });
        if (!ext_result) {
            return Result<TestSeq>::Err(ext_result.error());
        }
    }

    return Result<TestSeq>::Ok(v);
}

} // namespace

TEST(WorkedExampleSequence, RoundTripAllFieldsPresent) {
    std::array<std::byte, 8> buf{};
    UperWriter w(buf);
    const TestSeq original{50, 500, true, 7};
    ASSERT_TRUE(EncodeTestSeq(w, original));

    UperReader r(buf);
    auto decoded = DecodeTestSeq(r);
    ASSERT_TRUE(decoded.ok());
    EXPECT_EQ(decoded.value().a, 50);
    EXPECT_EQ(decoded.value().b, 500);
    ASSERT_TRUE(decoded.value().c.has_value());
    EXPECT_TRUE(*decoded.value().c);
    ASSERT_TRUE(decoded.value().d.has_value());
    EXPECT_EQ(*decoded.value().d, 7);
}

TEST(WorkedExampleSequence, RoundTripAllOptionalsAbsent) {
    std::array<std::byte, 8> buf{};
    UperWriter w(buf);
    const TestSeq original{1, 2, std::nullopt, std::nullopt};
    ASSERT_TRUE(EncodeTestSeq(w, original));
    EXPECT_EQ(w.bits_written(), 1u + 1u + 7u + 10u); // ext(0)+preamble(0)+a(7 bits, span=100)+b(10 bits, span=1000)

    UperReader r(buf);
    auto decoded = DecodeTestSeq(r);
    ASSERT_TRUE(decoded.ok());
    EXPECT_EQ(decoded.value().a, 1);
    EXPECT_EQ(decoded.value().b, 2);
    EXPECT_FALSE(decoded.value().c.has_value());
    EXPECT_FALSE(decoded.value().d.has_value());
}

TEST(WorkedExampleSequence, RoundTripOnlyOptionalPresent) {
    std::array<std::byte, 8> buf{};
    UperWriter w(buf);
    const TestSeq original{10, 20, false, std::nullopt};
    ASSERT_TRUE(EncodeTestSeq(w, original));

    UperReader r(buf);
    auto decoded = DecodeTestSeq(r);
    ASSERT_TRUE(decoded.ok());
    ASSERT_TRUE(decoded.value().c.has_value());
    EXPECT_FALSE(*decoded.value().c);
    EXPECT_FALSE(decoded.value().d.has_value());
}

TEST(WorkedExampleSequence, RoundTripOnlyExtensionPresent) {
    std::array<std::byte, 8> buf{};
    UperWriter w(buf);
    const TestSeq original{10, 20, std::nullopt, 3};
    ASSERT_TRUE(EncodeTestSeq(w, original));

    UperReader r(buf);
    auto decoded = DecodeTestSeq(r);
    ASSERT_TRUE(decoded.ok());
    EXPECT_FALSE(decoded.value().c.has_value());
    ASSERT_TRUE(decoded.value().d.has_value());
    EXPECT_EQ(*decoded.value().d, 3);
}
