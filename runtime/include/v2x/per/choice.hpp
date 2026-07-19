#pragma once

#include <cstdint>
#include <span>
#include <utility>

#include "v2x/per/error.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"

namespace v2x::per {

// CHOICE root alternative: extension bit (false) + constrained_whole_number
// index selection. The caller encodes the selected alternative's content
// directly against `w` afterward — no wrapping, unlike extension
// alternatives (design doc §4.3).
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_choice_root_index(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t index, uint32_t n_root_alts) {
    auto ext = w.bit(false);
    if (!ext) {
        return ext;
    }
    if (n_root_alts == 0) {
        return Status<>::Err(Error::InvalidConstraint);
    }
    return w.constrained_whole_number(static_cast<int64_t>(index), 0, static_cast<int64_t>(n_root_alts) - 1);
}

// CHOICE extension alternative: extension bit (true) + normally_small_length
// (ext_index directly — normally_small_length's own §11.6 small form already
// handles 0 correctly, no off-by-one adjustment needed here) +
// open_type-wrapped content (encode_fn runs against an inner writer over
// `scratch` — see open_type_wrap.hpp). ext_index is 0-based.
template <typename AlignPolicy, typename CanonicalPolicy, typename EncodeFn>
Status<> write_choice_extension(PerWriter<AlignPolicy, CanonicalPolicy>& w, uint32_t ext_index,
                                 std::span<std::byte> scratch, EncodeFn&& encode_fn) {
    auto ext = w.bit(true);
    if (!ext) {
        return ext;
    }
    auto idx = w.normally_small_length(ext_index);
    if (!idx) {
        return idx;
    }
    return write_open_type_wrapped(w, scratch, std::forward<EncodeFn>(encode_fn));
}

// Decode dispatch result: either a root alternative (is_extension=false,
// index selects which — the caller then decodes its content directly
// against the same reader, no unwrapping needed) or an extension
// alternative (is_extension=true, index is 0-based — the caller then calls
// read_open_type_wrapped directly to decode the wrapped content; no
// dedicated wrapper here, since it would add nothing over the existing
// open_type_wrap.hpp function).
struct ChoiceSelector {
    bool is_extension;
    uint32_t index;
};

template <typename AlignPolicy, typename CanonicalPolicy>
Result<ChoiceSelector> read_choice_selector(PerReader<AlignPolicy, CanonicalPolicy>& r, uint32_t n_root_alts) {
    auto ext = r.bit();
    if (!ext) {
        return Result<ChoiceSelector>::Err(ext.error());
    }
    if (!ext.value()) {
        if (n_root_alts == 0) {
            return Result<ChoiceSelector>::Err(Error::InvalidConstraint);
        }
        auto idx = r.constrained_whole_number(0, static_cast<int64_t>(n_root_alts) - 1);
        if (!idx) {
            return Result<ChoiceSelector>::Err(idx.error());
        }
        return Result<ChoiceSelector>::Ok(ChoiceSelector{false, static_cast<uint32_t>(idx.value())});
    }
    auto idx = r.normally_small_length();
    if (!idx) {
        return Result<ChoiceSelector>::Err(idx.error());
    }
    return Result<ChoiceSelector>::Ok(ChoiceSelector{true, idx.value()});
}

} // namespace v2x::per
