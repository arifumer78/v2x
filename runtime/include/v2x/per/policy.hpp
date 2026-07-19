#pragma once

#include "v2x/per/bit_cursor.hpp"

namespace v2x::per {

// Alignment affects only where octet-padding is inserted (before length
// determinants, before OCTET/BIT STRING content above a threshold, before
// open-type content) — implemented as a compile-time hook, never a runtime
// branch or a duplicated codec. UPER (Unaligned) is the primary target; Aligned
// exists for future APER reuse at effectively zero extra maintenance cost.
//
// Current wiring status (this phase only validates UPER — see uper.hpp):
// PerWriter calls AlignPolicy::align() at exactly one call site, in finish().
// The X.691-specified insertion points before length determinants and before
// OCTET/BIT STRING/open-type content are NOT yet wired in — Unaligned::align()
// being a no-op means this doesn't affect UPER correctness, but it does mean
// PerWriter<Aligned, ...> does not yet produce correct APER output beyond
// final padding. PerReader never calls align() at all yet, so
// PerReader<Aligned, ...> is currently behaviorally identical to
// PerReader<Unaligned, ...>. Both are real gaps to close before Aligned is
// anything more than "compiles, exists for the future."
struct Unaligned {
    static void align(BitCursor&) {}
};

struct Aligned {
    static void align(BitCursor& c) { c.pad_to_octet(); }
};

// Canonicality affects component/element ordering (SET, SET OF) and forces
// reserved padding bits to zero — orchestration-level concerns (compiler
// codegen), not primitive-level bit-packing. Basic is the primary target;
// Canonical is a marker type for now (see design doc §5.2/§11 — ETSI ITS
// modules are believed to be all SEQUENCE/SEQUENCE OF, so the SET-OF sort
// machinery this would eventually require is deferred, not implemented here).
struct Basic {};
struct Canonical {};

} // namespace v2x::per
