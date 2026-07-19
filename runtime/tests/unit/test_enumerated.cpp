#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>

#include "v2x/per/enumerated.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_enumerated;
using v2x::per::Unaligned;
using v2x::per::write_enumerated_extension;
using v2x::per::write_enumerated_root;

TEST(Enumerated, NonExtensibleRootKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_root(w, 3, 5, false)); // span=4, bit_width=3 (0..4 needs 3 bits)
    EXPECT_EQ(w.bits_written(), 3u);
}

TEST(Enumerated, ExtensibleRootKnownAnswer) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_root(w, 3, 5, true));
    EXPECT_EQ(w.bits_written(), 4u); // 1 (extension bit, false) + 3 (root index)
}

TEST(Enumerated, ExtensionValueKnownAnswer) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_extension(w, 0));
    EXPECT_EQ(w.bits_written(), 8u); // 1 (extension bit, true) + 7 (normally_small_length small form)
}

TEST(Enumerated, InvalidConstraintZeroRootValuesFails) {
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    auto status = write_enumerated_root(w, 0, 0, false);
    ASSERT_FALSE(status.ok());
    EXPECT_EQ(status.error(), Error::InvalidConstraint);
}

TEST(Enumerated, RoundTripRootValue) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_root(w, 4, 10, true));

    PerReader<Unaligned> r(buf);
    auto result = read_enumerated(r, 10, true);
    ASSERT_TRUE(result.ok());
    EXPECT_FALSE(result.value().is_extension);
    EXPECT_EQ(result.value().index, 4u);
}

TEST(Enumerated, RoundTripNonExtensibleRootValue) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_root(w, 2, 3, false));

    PerReader<Unaligned> r(buf);
    auto result = read_enumerated(r, 3, false);
    ASSERT_TRUE(result.ok());
    EXPECT_FALSE(result.value().is_extension);
    EXPECT_EQ(result.value().index, 2u);
}

TEST(Enumerated, RoundTripExtensionValueConfirmsNoContentWrapping) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_extension(w, 5));
    // 1 (ext bit) + 7 (small-form normally_small_length) = 8 bits total — if
    // this were open_type-wrapped like a CHOICE extension alternative, it
    // would need at least another 8-bit length_determinant byte on top.
    EXPECT_EQ(w.bits_written(), 8u);

    PerReader<Unaligned> r(buf);
    auto result = read_enumerated(r, 10, true);
    ASSERT_TRUE(result.ok());
    EXPECT_TRUE(result.value().is_extension);
    EXPECT_EQ(result.value().index, 5u);
}

TEST(Enumerated, RoundTripExtensionIndexZero) {
    // ext_index=0 is a legitimate, common case (the first extension value) —
    // normally_small_length's §11.6 small form handles 0 directly, no longer
    // treated as malformed (an earlier version of this library reserved 0 as
    // impossible due to a since-fixed off-by-one bug; see docs/CHANGELOG.md).
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_enumerated_extension(w, 0));

    PerReader<Unaligned> r(buf);
    auto result = read_enumerated(r, 10, true);
    ASSERT_TRUE(result.ok());
    EXPECT_TRUE(result.value().is_extension);
    EXPECT_EQ(result.value().index, 0u);
}
