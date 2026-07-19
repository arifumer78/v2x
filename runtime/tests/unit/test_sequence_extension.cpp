#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/sequence_extension.hpp"

using v2x::per::Error;
using v2x::per::PerReader;
using v2x::per::PerWriter;
using v2x::per::read_sequence_extension;
using v2x::per::Status;
using v2x::per::Unaligned;
using v2x::per::write_sequence_extension;

TEST(SequenceExtension, NoExtensionUsedIsJustABit) {
    // The "no extension" case isn't this helper's concern at all — the
    // caller writes the extension bit directly and never calls
    // write_sequence_extension / read_sequence_extension.
    std::array<std::byte, 1> buf{};
    PerWriter<Unaligned> w(buf);
    ASSERT_TRUE(w.bit(false));
    EXPECT_EQ(w.bits_written(), 1u);

    PerReader<Unaligned> r(buf);
    auto ext = r.bit();
    ASSERT_TRUE(ext.ok());
    EXPECT_FALSE(ext.value());
}

TEST(SequenceExtension, AllAdditionsAbsentRoundTrip) {
    std::array<std::byte, 4> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    const std::array<bool, 3> presence{false, false, false};
    // The extension bit is the caller's own responsibility (see
    // NoExtensionUsedIsJustABit) — write_sequence_extension only handles
    // what comes after it, so this test omits it entirely, same as the
    // library functions being tested.
    auto status = write_sequence_extension(w, presence, scratch,
                                            [](auto&, size_t) -> Status<> { return Status<>::Ok(); });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    bool decode_called = false;
    auto result = read_sequence_extension(r, 3, read_scratch, [&](auto&, uint32_t) -> Status<> {
        decode_called = true;
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u);
    EXPECT_FALSE(decode_called);
}

TEST(SequenceExtension, SomePresentRoundTrip) {
    std::array<std::byte, 16> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    const std::array<bool, 3> presence{true, false, true};
    const std::array<int64_t, 3> values{10, 0, 30}; // index 1 absent, value irrelevant
    auto status = write_sequence_extension(w, presence, scratch, [&](auto& inner, size_t idx) -> Status<> {
        return inner.constrained_whole_number(values[idx], 0, 1000);
    });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    std::array<int64_t, 3> decoded{-1, -1, -1};
    std::array<bool, 3> decoded_present{false, false, false};
    auto result = read_sequence_extension(r, 3, read_scratch, [&](auto& inner, uint32_t idx) -> Status<> {
        auto v = inner.constrained_whole_number(0, 1000);
        if (!v) {
            return Status<>::Err(v.error());
        }
        decoded[idx] = v.value();
        decoded_present[idx] = true;
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u);
    EXPECT_TRUE(decoded_present[0]);
    EXPECT_FALSE(decoded_present[1]);
    EXPECT_TRUE(decoded_present[2]);
    EXPECT_EQ(decoded[0], 10);
    EXPECT_EQ(decoded[2], 30);
}

TEST(SequenceExtension, UnknownAdditionBeyondKnownCountIsSkipped) {
    // Sender declares 3 additions, all present; this decoder only knows
    // about the first (known_count=1) — forward-compatible decoding: the
    // other two must be consumed but not decoded, not treated as an error.
    std::array<std::byte, 16> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    const std::array<bool, 3> presence{true, true, true};
    auto status = write_sequence_extension(w, presence, scratch, [](auto& inner, size_t idx) -> Status<> {
        return inner.constrained_whole_number(static_cast<int64_t>(idx) + 100, 0, 1000);
    });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    uint32_t decode_calls = 0;
    auto result = read_sequence_extension(r, /*known_count=*/1, read_scratch, [&](auto& inner, uint32_t idx) -> Status<> {
        ++decode_calls;
        EXPECT_EQ(idx, 0u);
        auto v = inner.constrained_whole_number(0, 1000);
        if (!v) {
            return Status<>::Err(v.error());
        }
        EXPECT_EQ(v.value(), 100);
        return Status<>::Ok();
    });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value(), 3u); // wire's declared M, even though this decoder only knows 1
    EXPECT_EQ(decode_calls, 1u);
}

TEST(SequenceExtension, ExtensionAdditionGroupNestedPreamble) {
    // X.691 §19.9: "Each extension addition that is an ExtensionAdditionGroup
    // shall be encoded as a sequence type as specified in 19.2 to 19.6 [i.e.
    // with its own inner preamble bitmap for its own OPTIONAL/DEFAULT
    // members], which is then encoded as if it were the value of an open
    // type field." Our generic callback design supports this without any
    // code changes: the encode_addition callback for a group-shaped slot
    // just writes an inner preamble bit plus its members before returning,
    // exactly like a normal SEQUENCE would — this test proves it, since
    // nothing about write/read_sequence_extension needed to change.
    //
    // Addition slot 0: plain ComponentType, INTEGER(0..10).
    // Addition slot 1: ExtensionAdditionGroup { c INTEGER(0..5), d BOOLEAN OPTIONAL }.
    std::array<std::byte, 16> buf{};
    std::array<std::byte, 16> scratch{};
    PerWriter<Unaligned> w(buf);
    const std::array<bool, 2> presence{true, true};
    constexpr int64_t b_value = 7;
    constexpr int64_t c_value = 3;
    constexpr bool d_present = true;
    constexpr bool d_value = true;

    auto status = write_sequence_extension(w, presence, scratch, [&](auto& inner, size_t idx) -> Status<> {
        if (idx == 0) {
            return inner.constrained_whole_number(b_value, 0, 10);
        }
        // idx == 1: the ExtensionAdditionGroup, encoded as a nested SEQUENCE
        // with its own preamble bit for the OPTIONAL member d.
        auto pb = inner.bit(d_present);
        if (!pb) {
            return pb;
        }
        auto cs = inner.constrained_whole_number(c_value, 0, 5);
        if (!cs) {
            return cs;
        }
        if (d_present) {
            return inner.bit(d_value);
        }
        return Status<>::Ok();
    });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 16> read_scratch{};
    int64_t decoded_b = -1;
    int64_t decoded_c = -1;
    bool decoded_d_present = false;
    bool decoded_d = false;
    auto result =
        read_sequence_extension(r, /*known_count=*/2, read_scratch, [&](auto& inner, uint32_t idx) -> Status<> {
            if (idx == 0) {
                auto v = inner.constrained_whole_number(0, 10);
                if (!v) {
                    return Status<>::Err(v.error());
                }
                decoded_b = v.value();
                return Status<>::Ok();
            }
            auto pb = inner.bit();
            if (!pb) {
                return Status<>::Err(pb.error());
            }
            decoded_d_present = pb.value();
            auto cv = inner.constrained_whole_number(0, 5);
            if (!cv) {
                return Status<>::Err(cv.error());
            }
            decoded_c = cv.value();
            if (decoded_d_present) {
                auto dv = inner.bit();
                if (!dv) {
                    return Status<>::Err(dv.error());
                }
                decoded_d = dv.value();
            }
            return Status<>::Ok();
        });
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(decoded_b, b_value);
    EXPECT_EQ(decoded_c, c_value);
    EXPECT_EQ(decoded_d_present, d_present);
    EXPECT_EQ(decoded_d, d_value);
}

TEST(SequenceExtension, TooManyAdditionsOnDecodeFails) {
    std::vector<std::byte> buf(50);
    std::array<std::byte, 4> scratch{};
    PerWriter<Unaligned> w(buf);
    std::array<bool, 100> presence{}; // M=100, exceeds read_sequence_extension's 64 cap (all false)
    auto status = write_sequence_extension(w, presence, scratch,
                                            [](auto&, size_t) -> Status<> { return Status<>::Ok(); });
    ASSERT_TRUE(status.ok());

    PerReader<Unaligned> r(buf);
    std::array<std::byte, 4> read_scratch{};
    auto result = read_sequence_extension(r, 0, read_scratch, [](auto&, uint32_t) -> Status<> { return Status<>::Ok(); });
    ASSERT_FALSE(result.ok());
    EXPECT_EQ(result.error(), Error::LengthTooLarge);
}
