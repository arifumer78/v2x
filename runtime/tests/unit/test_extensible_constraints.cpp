#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "v2x/per/bit_string.hpp"
#include "v2x/per/integer.hpp"
#include "v2x/per/octet_string.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/sequence_of.hpp"
#include "v2x/per/size_range.hpp"

// Constraint-level extensibility (e.g. `INTEGER (0..10, ...)`), distinct
// from SEQUENCE/CHOICE/ENUMERATED's structural extensibility — X.691 §13.1,
// §16.6, §17.3, §20.4. All four follow the same shape: extension bit, then
// either the normal root-constrained encoding or a general/unconstrained
// fallback if the value falls outside the extension root.

using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_extensible_bit_string;
using v2x::per::read_extensible_integer;
using v2x::per::read_extensible_octet_string;
using v2x::per::read_extensible_sequence_of;
using v2x::per::SizeRange;
using v2x::per::Status;
using v2x::per::Unaligned;
using v2x::per::write_extensible_bit_string;
using v2x::per::write_extensible_integer;
using v2x::per::write_extensible_octet_string;
using v2x::per::write_extensible_sequence_of;

TEST(ExtensibleInteger, InRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_extensible_integer(w, 5, 0, 10)); // span=10, bit_width=4
    EXPECT_EQ(w.bits_written(), 1u + 4u);                // ext(0) + 4 bits

    PerReader<Unaligned> r(buf);
    auto result = read_extensible_integer(r, 0, 10);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 5);
}

TEST(ExtensibleInteger, OutOfRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 4> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(write_extensible_integer(w, 20, 0, 10)); // out of [0,10]
    EXPECT_EQ(w.bits_written(), 1u + 16u);                // ext(1) + unconstrained_whole_number(20): 8(len)+8(content)

    PerReader<Unaligned> r(buf);
    auto result = read_extensible_integer(r, 0, 10);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 20);
}

TEST(ExtensibleOctetString, InRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 3> content{std::byte{1}, std::byte{2}, std::byte{3}};
    const SizeRange root{1, 5}; // span=4, bit_width=3
    ASSERT_TRUE(write_extensible_octet_string(w, content, root));
    EXPECT_EQ(w.bits_written(), 1u + 3u + 24u);

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 3> out{};
    auto result = read_extensible_octet_string(r, out, root);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u);
    EXPECT_EQ(out, content);
}

TEST(ExtensibleOctetString, OutOfRootKnownAnswerAndRoundTrip) {
    std::vector<std::byte> content(10);
    for (size_t i = 0; i < content.size(); ++i) {
        content[i] = static_cast<std::byte>(i);
    }
    std::vector<std::byte> buf(20);
    PerWriter<Unaligned> w(buf);
    const SizeRange root{1, 5}; // 10 is outside [1,5]
    ASSERT_TRUE(write_extensible_octet_string(w, content, root));
    EXPECT_EQ(w.bits_written(), 1u + 8u + 80u); // ext(1) + short-form length byte + 10*8 content bits

    PerReader<Unaligned> r(buf);
    std::vector<std::byte> out(10);
    auto result = read_extensible_octet_string(r, out, root);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 10u);
    EXPECT_EQ(out, content);
}

TEST(ExtensibleBitString, InRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::array<std::byte, 2> content{std::byte{0xB4}, std::byte{0xA0}};
    const SizeRange root{1, 20}; // span=19, bit_width=5
    ASSERT_TRUE(write_extensible_bit_string(w, content, 13, root));
    EXPECT_EQ(w.bits_written(), 1u + 5u + 13u);

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 2> out{};
    auto result = read_extensible_bit_string(r, out, root);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 13u);
    EXPECT_EQ(out, content);
}

TEST(ExtensibleBitString, OutOfRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    std::array<std::byte, 4> content{std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0x80}};
    const SizeRange root{1, 20}; // 25 bits is outside [1,20]
    ASSERT_TRUE(write_extensible_bit_string(w, content, 25, root));
    EXPECT_EQ(w.bits_written(), 1u + 8u + 25u); // ext(1) + short-form length byte + 25 content bits

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 4> out{};
    auto result = read_extensible_bit_string(r, out, root);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 25u);
}

namespace {
Status<> EncodeByteElement(PerWriter<Unaligned>& w, uint32_t index, const std::vector<int64_t>& values) {
    return w.constrained_whole_number(values[index], 0, 255);
}
} // namespace

TEST(ExtensibleSequenceOf, InRootKnownAnswerAndRoundTrip) {
    std::array<std::byte, 8> buf{};
    PerWriter<Unaligned> w(buf);
    const std::vector<int64_t> values{1, 2, 3};
    const SizeRange root{1, 5}; // span=4, bit_width=3
    ASSERT_TRUE(write_extensible_sequence_of(w, 3, root,
                                              [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); }));
    EXPECT_EQ(w.bits_written(), 1u + 3u + 24u);

    PerReader<Unaligned> r(buf);
    std::vector<int64_t> decoded;
    auto result = read_extensible_sequence_of(r, root, [&](auto& inner, uint32_t) -> Status<> {
        auto v = inner.constrained_whole_number(0, 255);
        if (!v) {
            return Status<>::Err(v.error());
        }
        decoded.push_back(v.value());
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u);
    EXPECT_EQ(decoded, values);
}

TEST(ExtensibleSequenceOf, OutOfRootKnownAnswerAndRoundTrip) {
    std::vector<int64_t> values;
    for (int i = 0; i < 10; ++i) {
        values.push_back(i);
    }
    std::vector<std::byte> buf(20);
    PerWriter<Unaligned> w(buf);
    const SizeRange root{1, 5}; // 10 elements is outside [1,5]
    ASSERT_TRUE(write_extensible_sequence_of(w, static_cast<uint32_t>(values.size()), root,
                                              [&](auto& inner, uint32_t idx) { return EncodeByteElement(inner, idx, values); }));
    EXPECT_EQ(w.bits_written(), 1u + 8u + 80u); // ext(1) + short-form length byte + 10*8 element bits

    PerReader<Unaligned> r(buf);
    std::vector<int64_t> decoded;
    auto result = read_extensible_sequence_of(r, root, [&](auto& inner, uint32_t) -> Status<> {
        auto v = inner.constrained_whole_number(0, 255);
        if (!v) {
            return Status<>::Err(v.error());
        }
        decoded.push_back(v.value());
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 10u);
    EXPECT_EQ(decoded, values);
}
