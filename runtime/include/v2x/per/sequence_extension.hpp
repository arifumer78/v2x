#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>

#include "v2x/per/error.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"

namespace v2x::per {

// SEQUENCE extension-addition group: normally_small_length(M) for the
// addition-group size, M presence bits, then each present addition
// individually open_type-wrapped (design doc §4.3). presence.size() is M —
// the caller's own compile-time-known addition count.
//
// The extension bit itself is the CALLER's responsibility on both the write
// and read sides (write it/read it before deciding whether to call this
// function at all) — this helper only handles what comes after it, so it
// stays symmetric with a decoder that must inspect the bit before knowing
// whether an addition group follows.
template <typename AlignPolicy, typename CanonicalPolicy, typename EncodeAdditionFn>
Status<> write_sequence_extension(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<const bool> presence,
                                   std::span<std::byte> scratch, EncodeAdditionFn&& encode_addition) {
    auto count_status = w.normally_small_length(static_cast<uint32_t>(presence.size()));
    if (!count_status) {
        return count_status;
    }
    for (const bool p : presence) {
        auto pb = w.bit(p);
        if (!pb) {
            return pb;
        }
    }
    for (size_t i = 0; i < presence.size(); ++i) {
        if (!presence[i]) {
            continue;
        }
        auto s = write_open_type_wrapped(
            w, scratch, [&](auto& inner) { return std::forward<EncodeAdditionFn>(encode_addition)(inner, i); });
        if (!s) {
            return s;
        }
    }
    return Status<>::Ok();
}

// Mirrors write_sequence_extension: the caller reads the extension bit
// itself and only calls this once it's true.
//
// known_count is THIS decoder's own compiled-in number of addition slots it
// understands — which may be less than the wire's actual declared count M
// if the sender uses a newer spec version with more extension additions than
// this decoder knows about. Per ASN.1 forward-compatible extensibility
// semantics, that's not an error: present additions at index >= known_count
// are consumed (their open_type-wrapped bytes skipped via `scratch`, reusing
// it as throwaway space) but not decoded. Returns M, the wire's declared
// addition-group size, letting the caller detect this situation if it cares.
//
// M is capped at 64 (close to, though not tied to, normally_small_length's
// own small-form threshold of 63 — a generous bound for realistic ETSI ITS
// addition-group sizes either way) so the presence bitmap can live in fixed
// on-stack storage rather than needing dynamic allocation for a
// wire-controlled count; larger M fails with Error::LengthTooLarge rather
// than allocating or truncating silently.
template <typename AlignPolicy, typename CanonicalPolicy, typename DecodeAdditionFn>
Result<uint32_t> read_sequence_extension(PerReader<AlignPolicy, CanonicalPolicy>& r, uint32_t known_count,
                                          std::span<std::byte> scratch, DecodeAdditionFn&& decode_addition) {
    constexpr uint32_t kMaxAdditions = 64;

    auto count_result = r.normally_small_length();
    if (!count_result) {
        return Result<uint32_t>::Err(count_result.error());
    }
    const uint32_t m = count_result.value();
    if (m > kMaxAdditions) {
        return Result<uint32_t>::Err(Error::LengthTooLarge);
    }

    // Presence bits form one contiguous block on the wire, before any
    // content — must be fully read before any addition is processed.
    std::array<bool, kMaxAdditions> presence{};
    for (uint32_t i = 0; i < m; ++i) {
        auto p = r.bit();
        if (!p) {
            return Result<uint32_t>::Err(p.error());
        }
        presence[i] = p.value();
    }

    for (uint32_t i = 0; i < m; ++i) {
        if (!presence[i]) {
            continue;
        }
        if (i < known_count) {
            auto s = read_open_type_wrapped(r, scratch, [&](auto& inner) -> Status<> {
                return std::forward<DecodeAdditionFn>(decode_addition)(inner, i);
            });
            if (!s) {
                return Result<uint32_t>::Err(s.error());
            }
        } else {
            auto skip = r.open_type(scratch); // unknown addition — consume, don't decode
            if (!skip) {
                return Result<uint32_t>::Err(skip.error());
            }
        }
    }
    return Result<uint32_t>::Ok(m);
}

} // namespace v2x::per
