#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include "v2x/per/choice.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::ChoiceSelector;
using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_choice_selector;
using v2x::per::read_open_type_wrapped;
using v2x::per::Status;
using v2x::per::Unaligned;
using v2x::per::write_choice_extension;
using v2x::per::write_choice_root_index;

TEST(Choice, RootAlternativeKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_choice_root_index(w, 1, 3)); // span=2, bit_width=2
    EXPECT_EQ(w.bits_written(), 3u);                // 1 (ext bit) + 2 (index)
}

TEST(Choice, InvalidConstraintZeroRootAltsFails) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = write_choice_root_index(w, 0, 0);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::InvalidConstraint);
}

TEST(Choice, RoundTripRootAlternative) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_choice_root_index(w, 2, 5));
    ASSERT_TRUE(w.constrained_whole_number(777, 0, 1000)); // the selected alternative's content

    PerReader<Unaligned> r(buf);
    auto selector = read_choice_selector(r, 5);
    ASSERT_TRUE(selector.ok());
    EXPECT_FALSE(selector.value().is_extension);
    EXPECT_EQ(selector.value().index, 2u);
    auto content = r.constrained_whole_number(0, 1000); // no unwrapping — direct read
    ASSERT_TRUE(content.ok());
    EXPECT_EQ(content.value(), 777);
}

TEST(Choice, ExtensionAlternativeSelectorKnownAnswerBitCount) {
    std::array<std::byte, 4> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_choice_extension(w, 0, scratch,
                                        [](auto& inner) { return inner.constrained_whole_number(5, 0, 15); }));
    // selector: 1 (ext bit) + 7 (small-form normally_small_length) = 8 bits,
    // then the open_type-wrapped content adds at least 8 more (length byte).
    EXPECT_GE(w.bits_written(), 8u + 8u);
}

TEST(Choice, RoundTripExtensionAlternative) {
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_choice_extension(w, 2, scratch,
                                        [](auto& inner) { return inner.constrained_whole_number(99, 0, 255); }));

    PerReader<Unaligned> r(buf);
    auto selector = read_choice_selector(r, 5);
    ASSERT_TRUE(selector.ok());
    EXPECT_TRUE(selector.value().is_extension);
    EXPECT_EQ(selector.value().index, 2u);

    std::array<std::byte, 16> read_scratch{};
    auto content = read_open_type_wrapped(r, read_scratch,
                                           [](auto& inner) { return inner.constrained_whole_number(0, 255); });
    ASSERT_TRUE(content.ok());
    EXPECT_EQ(content.value(), 99);
}

TEST(Choice, RoundTripExtensionAlternativeIndexZero) {
    // ext_index=0 is a legitimate, common case (the first extension
    // alternative) — normally_small_length's §11.6 small form handles 0
    // directly, no longer treated as malformed (an earlier version of this
    // library reserved 0 as impossible due to a since-fixed off-by-one bug;
    // see docs/CHANGELOG.md).
    std::array<std::byte, 8> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_choice_extension(w, 0, scratch,
                                        [](auto& inner) { return inner.constrained_whole_number(7, 0, 15); }));

    PerReader<Unaligned> r(buf);
    auto selector = read_choice_selector(r, 5);
    ASSERT_TRUE(selector.ok());
    EXPECT_TRUE(selector.value().is_extension);
    EXPECT_EQ(selector.value().index, 0u);
}

TEST(Choice, MultipleRootAlternativesRoundTripByIndex) {
    // A small hand-written CHOICE-like dispatch: 3 root alternatives, each
    // with a different content type/constraint, dispatched by index — the
    // pattern a real hand-written CHOICE type would follow.
    for (uint32_t alt_index : {0u, 1u, 2u}) {
        std::array<std::byte, 4> buf{};
        PerWriter<Unaligned> w(buf);
        ASSERT_TRUE(write_choice_root_index(w, alt_index, 3));
        switch (alt_index) {
            case 0:
                ASSERT_TRUE(w.bit(true));
                break;
            case 1:
                ASSERT_TRUE(w.constrained_whole_number(42, 0, 100));
                break;
            case 2:
                ASSERT_TRUE(w.unconstrained_whole_number(-7));
                break;
        }

        PerReader<Unaligned> r(buf);
        auto selector = read_choice_selector(r, 3);
        ASSERT_TRUE(selector.ok());
        EXPECT_FALSE(selector.value().is_extension);
        EXPECT_EQ(selector.value().index, alt_index);
        switch (selector.value().index) {
            case 0: {
                auto v = r.bit();
                ASSERT_TRUE(v.ok());
                EXPECT_TRUE(v.value());
                break;
            }
            case 1: {
                auto v = r.constrained_whole_number(0, 100);
                ASSERT_TRUE(v.ok());
                EXPECT_EQ(v.value(), 42);
                break;
            }
            case 2: {
                auto v = r.unconstrained_whole_number();
                ASSERT_TRUE(v.ok());
                EXPECT_EQ(v.value(), -7);
                break;
            }
        }
    }
}
