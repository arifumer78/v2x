#pragma once

#include <cstdint>
#include <optional>
#include <utility>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/size_range.hpp"

namespace v2x::per {

// SEQUENCE OF / SET OF: length_determinant (element count, with fragmentation)
// then per-element encode — X.691, design doc §4.3. Same chunk protocol as
// write_length_prefixed_content, but the per-chunk "content" is a run of
// caller-encoded elements (arbitrary width) rather than raw bits, so it can't
// share that helper directly — it calls back into encode_element once per
// element instead.
template <typename AlignPolicy, typename CanonicalPolicy, typename EncodeElementFn>
Status<> write_sequence_of(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t count,
                            std::optional<SizeRange> constraint, EncodeElementFn&& encode_element) {
    uint32_t index = 0;
    uint32_t remaining = count;
    while (true) {
        auto ld = w.length_determinant(remaining, constraint);
        if (!ld) {
            return Status<>::Err(ld.error());
        }
        const uint32_t chunk = ld.value();
        for (uint32_t i = 0; i < chunk; ++i) {
            auto es = encode_element(w, index);
            if (!es) {
                return es;
            }
            ++index;
        }
        remaining -= chunk;
        if (chunk < 16384) {
            break;
        }
    }
    return Status<>::Ok();
}

// Returns the total number of elements decoded. decode_element is called once
// per element with its 0-based index; it MUST bounds-check that index against
// the caller's own fixed-capacity storage and return an error if exceeded —
// this function has no storage of its own and does not itself cap the total
// element count (mirrors PerReader::open_type's caller-provided-destination
// pattern — a SEQUENCE OF an always-zero-bit element type, e.g. NULL, would
// otherwise let a malformed/adversarial huge declared count run unbounded
// without ever exhausting the input buffer).
template <typename AlignPolicy, typename CanonicalPolicy, typename DecodeElementFn>
Result<uint32_t> read_sequence_of(PerReader<AlignPolicy, CanonicalPolicy>& r, std::optional<SizeRange> constraint,
                                   DecodeElementFn&& decode_element) {
    uint32_t index = 0;
    while (true) {
        auto ld = r.length_determinant(constraint);
        if (!ld) {
            return Result<uint32_t>::Err(ld.error());
        }
        const uint32_t chunk = ld.value();
        for (uint32_t i = 0; i < chunk; ++i) {
            auto es = decode_element(r, index);
            if (!es) {
                return Result<uint32_t>::Err(es.error());
            }
            ++index;
        }
        if (chunk < 16384) {
            break;
        }
    }
    return Result<uint32_t>::Ok(index);
}

// Extensible SEQUENCE OF / SET OF — X.691 §20.4 (constraint-level
// extensibility, e.g. `SEQUENCE (SIZE(1..10, ...)) OF Foo`). Same shape as
// write_extensible_octet_string (see its comment) — root_constraint bounds
// the element count, not a byte/bit count.
template <typename AlignPolicy, typename CanonicalPolicy, typename EncodeElementFn>
Status<> write_extensible_sequence_of(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t count,
                                       SizeRange root_constraint, EncodeElementFn&& encode_element) {
    const bool in_root = count >= root_constraint.min &&
                          (!root_constraint.max.has_value() || count <= *root_constraint.max);
    auto ext = w.bit(!in_root);
    if (!ext) {
        return ext;
    }
    if (in_root) {
        return write_sequence_of(w, count, root_constraint, std::forward<EncodeElementFn>(encode_element));
    }
    return write_sequence_of(w, count, std::nullopt, std::forward<EncodeElementFn>(encode_element));
}

template <typename AlignPolicy, typename CanonicalPolicy, typename DecodeElementFn>
Result<uint32_t> read_extensible_sequence_of(PerReader<AlignPolicy, CanonicalPolicy>& r, SizeRange root_constraint,
                                              DecodeElementFn&& decode_element) {
    auto ext = r.bit();
    if (!ext) {
        return Result<uint32_t>::Err(ext.error());
    }
    if (!ext.value()) {
        return read_sequence_of(r, root_constraint, std::forward<DecodeElementFn>(decode_element));
    }
    return read_sequence_of(r, std::nullopt, std::forward<DecodeElementFn>(decode_element));
}

} // namespace v2x::per
