#pragma once

#include <cstddef>
#include <span>
#include <type_traits>
#include <utility>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"

namespace v2x::per {

// Encodes one value into `scratch` via the caller-supplied encode_fn (which
// calls primitives against the inner PerWriter it's given), then
// open_type-wraps the resulting bytes into the outer writer `w`. Used for
// SEQUENCE extension-addition content and CHOICE extension-alternative
// content — both need already-encoded bytes for open_type but only have a
// value to encode. Given the "no dynamic allocation" posture, `scratch` is
// caller-provided (stack-allocated in hand-written code), sized generously
// enough for one field/alternative's max encoded size — a
// compile-time-computable bound once a compiler exists; hand-picked for now.
template <typename AlignPolicy, typename CanonicalPolicy, typename EncodeFn>
Status<> write_open_type_wrapped(PerWriter<AlignPolicy, CanonicalPolicy>& w, std::span<std::byte> scratch,
                                  EncodeFn&& encode_fn) {
    PerWriter<AlignPolicy, CanonicalPolicy> inner(scratch);
    auto inner_status = std::forward<EncodeFn>(encode_fn)(inner);
    if (!inner_status) {
        return inner_status;
    }
    return w.open_type(inner.finish());
}

// Decodes an open_type-wrapped value: unwraps into `scratch`, constructs an
// inner PerReader over exactly the decoded bytes, and invokes decode_fn
// against it. decode_fn's return type (some Result<T>/Status<E>) is deduced
// and forwarded directly — no separate template parameter needed.
template <typename AlignPolicy, typename CanonicalPolicy, typename DecodeFn>
auto read_open_type_wrapped(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> scratch,
                             DecodeFn&& decode_fn)
    -> std::invoke_result_t<DecodeFn, PerReader<AlignPolicy, CanonicalPolicy>&> {
    using ReturnType = std::invoke_result_t<DecodeFn, PerReader<AlignPolicy, CanonicalPolicy>&>;
    auto unwrapped = r.open_type(scratch);
    if (!unwrapped) {
        return ReturnType::Err(unwrapped.error());
    }
    PerReader<AlignPolicy, CanonicalPolicy> inner(scratch.subspan(0, unwrapped.value()));
    return std::forward<DecodeFn>(decode_fn)(inner);
}

} // namespace v2x::per
