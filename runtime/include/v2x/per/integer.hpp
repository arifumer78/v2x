#pragma once

#include <cstdint>

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/result.hpp"

namespace v2x::per {

// Extensible constrained INTEGER — X.691 §13.1. This is *constraint-level*
// extensibility (e.g. `INTEGER (0..10, ...)`), distinct from the
// *structural* extensibility SEQUENCE/CHOICE/ENUMERATED already have
// (addition groups / extension alternatives / extension values). If v falls
// within the extension root [lb, ub]: extension bit 0 + normal
// constrained_whole_number(v, lb, ub). Otherwise (only valid because the
// constraint is extensible): extension bit 1 + v encoded as an
// unconstrained whole number (§13.2.4, via unconstrained_whole_number) —
// per the spec text this is unconstrained, not semi-constrained, unlike the
// length-based constructs (octet_string/bit_string/sequence_of) below.
//
// Scope: assumes a finite extension-root range [lb, ub] (the common case
// for a hand-written `INTEGER (lb..ub, ...)`); an extensible
// semi-constrained or unconstrained root (`INTEGER (lb..MAX, ...)`) isn't
// covered — not needed by anything built so far.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_extensible_integer(PerWriter<AlignPolicy, CanonicalPolicy>& w, int64_t v, int64_t lb, int64_t ub) {
    if (v >= lb && v <= ub) {
        auto ext = w.bit(false);
        if (!ext) {
            return ext;
        }
        return w.constrained_whole_number(v, lb, ub);
    }
    auto ext = w.bit(true);
    if (!ext) {
        return ext;
    }
    return w.unconstrained_whole_number(v);
}

template <typename AlignPolicy, typename CanonicalPolicy>
Result<int64_t> read_extensible_integer(PerReader<AlignPolicy, CanonicalPolicy>& r, int64_t lb, int64_t ub) {
    auto ext = r.bit();
    if (!ext) {
        return Result<int64_t>::Err(ext.error());
    }
    if (!ext.value()) {
        return r.constrained_whole_number(lb, ub);
    }
    return r.unconstrained_whole_number();
}

} // namespace v2x::per
