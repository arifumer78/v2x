#pragma once

#include <cstdint>

#include "v2x/per/error.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"

namespace v2x::per {

// ENUMERATED root value: constrained_whole_number(index, 0, n_root_values-1),
// with an extension bit first if the type is extensible (design doc §4.3).
// Non-extensible ENUMERATED types have no extension bit at all — pass
// extensible=false for those.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_enumerated_root(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t index, uint32_t n_root_values,
                                bool extensible) {
    if (extensible) {
        auto ext = w.bit(false);
        if (!ext) {
            return ext;
        }
    }
    if (n_root_values == 0) {
        return Status<>::Err(Error::InvalidConstraint);
    }
    return w.constrained_whole_number(static_cast<int64_t>(index), 0, static_cast<int64_t>(n_root_values) - 1);
}

// ENUMERATED extension value: extension bit + normally_small_length(index) —
// normally_small_length's own §11.6 small form already handles 0 correctly,
// no off-by-one adjustment needed. Unlike CHOICE, an ENUMERATED extension
// value has no associated content to open_type-wrap — it's purely an
// identity, so this is the whole encoding. ext_index is 0-based (position
// among extension values, not root ones).
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_enumerated_extension(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t ext_index) {
    auto ext = w.bit(true);
    if (!ext) {
        return ext;
    }
    return w.normally_small_length(ext_index);
}

// Decode result: either a root value (is_extension=false, index is the root
// index) or an extension value (is_extension=true, index is the 0-based
// extension index).
struct EnumeratedValue {
    bool is_extension;
    uint32_t index;
};

// Combined read: decode genuinely doesn't know whether a root or extension
// value follows until the extension bit (if the type is extensible) is
// read, unlike the write side where the caller already knows which case
// applies. extensible must match what the writer used.
template <typename AlignPolicy, typename CanonicalPolicy>
Result<EnumeratedValue> read_enumerated(PerReader<AlignPolicy, CanonicalPolicy>& r, uint32_t n_root_values,
                                         bool extensible) {
    if (extensible) {
        auto ext = r.bit();
        if (!ext) {
            return Result<EnumeratedValue>::Err(ext.error());
        }
        if (ext.value()) {
            auto idx = r.normally_small_length();
            if (!idx) {
                return Result<EnumeratedValue>::Err(idx.error());
            }
            return Result<EnumeratedValue>::Ok(EnumeratedValue{true, idx.value()});
        }
    }
    if (n_root_values == 0) {
        return Result<EnumeratedValue>::Err(Error::InvalidConstraint);
    }
    auto root = r.constrained_whole_number(0, static_cast<int64_t>(n_root_values) - 1);
    if (!root) {
        return Result<EnumeratedValue>::Err(root.error());
    }
    return Result<EnumeratedValue>::Ok(EnumeratedValue{false, static_cast<uint32_t>(root.value())});
}

} // namespace v2x::per
