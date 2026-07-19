#pragma once

#include <cstddef>
#include <optional>
#include <span>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/size_range.hpp"

namespace v2x::per {

// OCTET STRING: length_determinant-prefixed raw content, counted in octets
// (unit_bits=8) — the same shape as open_type, but with an optional SIZE
// constraint. See PerWriter::write_length_prefixed_content for the shared
// fragmentation logic this composes.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_octet_string(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<const std::byte> content,
                             std::optional<SizeRange> constraint = std::nullopt) {
    return write_length_prefixed_content(w, content, content.size() * 8, 8, constraint);
}

// Returns the number of bytes written into out.
template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_octet_string(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                  std::optional<SizeRange> constraint = std::nullopt) {
    auto res = read_length_prefixed_content(r, out, 8, constraint);
    if (!res) {
        return res;
    }
    return Result<size_t>::Ok(res.value() / 8); // exact: unit_bits=8 keeps every chunk byte-aligned
}

// Extensible OCTET STRING — X.691 §17.3 (constraint-level extensibility,
// e.g. `OCTET STRING (SIZE(1..10, ...))`). If content's length falls within
// the extension root's SizeRange: extension bit 0 + write_octet_string with
// the root constraint (matches "as if the extension marker is not
// present"). Otherwise: extension bit 1 + write_octet_string with no
// constraint at all — §17.3 calls this "length as a semi-constrained whole
// number", which is exactly length_determinant's general/unconstrained form
// (no constrained-delegation), i.e. passing std::nullopt.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_extensible_octet_string(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<const std::byte> content,
                                        SizeRange root_constraint) {
    const size_t n = content.size();
    const bool in_root = n >= root_constraint.min &&
                          (!root_constraint.max.has_value() || n <= *root_constraint.max);
    auto ext = w.bit(!in_root);
    if (!ext) {
        return ext;
    }
    if (in_root) {
        return write_octet_string(w, content, root_constraint);
    }
    return write_octet_string(w, content, std::nullopt);
}

template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_extensible_octet_string(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                             SizeRange root_constraint) {
    auto ext = r.bit();
    if (!ext) {
        return Result<size_t>::Err(ext.error());
    }
    if (!ext.value()) {
        return read_octet_string(r, out, root_constraint);
    }
    return read_octet_string(r, out, std::nullopt);
}

} // namespace v2x::per
