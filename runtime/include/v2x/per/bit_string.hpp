#pragma once

#include <cstddef>
#include <optional>
#include <span>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/size_range.hpp"

namespace v2x::per {

// BIT STRING: length_determinant-prefixed raw content, counted in BITS
// (unit_bits=1) — unlike OCTET STRING/open_type, which count in octets. This
// is a real X.691 distinction, not a simplification: the fragmentation
// threshold (16384) applies to bits here, so a 16384-*bit* (2048-byte) BIT
// STRING fragments, not a 16384-*byte* one. n_bits need not be a multiple of
// 8 — content's trailing bits beyond n_bits are ignored on write and
// unspecified on read (see PerWriter::bits / PerReader::bits).
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_bit_string(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<const std::byte> content,
                           size_t n_bits, std::optional<SizeRange> constraint = std::nullopt) {
    return write_length_prefixed_content(w, content, n_bits, 1, constraint);
}

// Returns the number of BITS written into out (may not be a multiple of 8) —
// unlike read_octet_string/PerReader::open_type, no /8 conversion applies.
template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_bit_string(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                std::optional<SizeRange> constraint = std::nullopt) {
    return read_length_prefixed_content(r, out, 1, constraint);
}

// Extensible BIT STRING — X.691 §16.6 (constraint-level extensibility, e.g.
// `BIT STRING (SIZE(1..10, ...))`). Same shape as write_extensible_octet_string
// (see its comment) — root_constraint and n_bits are both counted in bits
// here, matching write_bit_string/read_bit_string's own convention.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_extensible_bit_string(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<const std::byte> content,
                                      size_t n_bits, SizeRange root_constraint) {
    const bool in_root = n_bits >= root_constraint.min &&
                          (!root_constraint.max.has_value() || n_bits <= *root_constraint.max);
    auto ext = w.bit(!in_root);
    if (!ext) {
        return ext;
    }
    if (in_root) {
        return write_bit_string(w, content, n_bits, root_constraint);
    }
    return write_bit_string(w, content, n_bits, std::nullopt);
}

template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_extensible_bit_string(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                           SizeRange root_constraint) {
    auto ext = r.bit();
    if (!ext) {
        return Result<size_t>::Err(ext.error());
    }
    if (!ext.value()) {
        return read_bit_string(r, out, root_constraint);
    }
    return read_bit_string(r, out, std::nullopt);
}

} // namespace v2x::per
