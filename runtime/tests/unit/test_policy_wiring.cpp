#include <gtest/gtest.h>

#include <array>
#include <cstddef>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/uper.hpp"

using v2x::per::Aligned;
using v2x::per::Basic;
using v2x::per::Canonical;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::Unaligned;
using v2x::per::UperReader;
using v2x::per::UperWriter;

template <typename AlignPolicy, typename CanonicalPolicy>
void ExerciseWriterInstantiation() {
    std::array<std::byte, 2> buf{};
    PerWriter<AlignPolicy, CanonicalPolicy> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.constrained_whole_number(3, 0, 7));
}

template <typename AlignPolicy, typename CanonicalPolicy>
void ExerciseReaderInstantiation() {
    std::array<std::byte, 2> buf{std::byte{0xFF}, std::byte{0xFF}};
    PerReader<AlignPolicy, CanonicalPolicy> r(buf);
    ASSERT_TRUE(r.bit());
    ASSERT_TRUE(r.constrained_whole_number(0, 7));
}

TEST(PolicyWiring, AllFourWriterCombinationsCompileAndWork) {
    ExerciseWriterInstantiation<Unaligned, Basic>();
    ExerciseWriterInstantiation<Aligned, Basic>();
    ExerciseWriterInstantiation<Unaligned, Canonical>();
    ExerciseWriterInstantiation<Aligned, Canonical>();
}

TEST(PolicyWiring, AllFourReaderCombinationsCompileAndWork) {
    ExerciseReaderInstantiation<Unaligned, Basic>();
    ExerciseReaderInstantiation<Aligned, Basic>();
    ExerciseReaderInstantiation<Unaligned, Canonical>();
    ExerciseReaderInstantiation<Aligned, Canonical>();
}

TEST(PolicyWiring, AlignedFinishPadsToNextOctetBoundary) {
    std::array<std::byte, 2> buf{};
    PerWriter<Aligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true)); // 3 bits
    w.finish();
    EXPECT_EQ(w.bits_written(), 8u); // padded up to the next octet boundary
}

TEST(PolicyWiring, UnalignedFinishDoesNotPad) {
    std::array<std::byte, 2> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true));
    ASSERT_TRUE(w.bit(true)); // 3 bits
    w.finish();
    EXPECT_EQ(w.bits_written(), 3u); // no padding in UPER
}

TEST(PolicyWiring, UperAliasesRoundTrip) {
    std::array<std::byte, 4> buf{};
    UperWriter w(buf);
    ASSERT_TRUE(w.constrained_whole_number(123, 0, 1000));

    UperReader r(buf);
    auto result = r.constrained_whole_number(0, 1000);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 123);
}
