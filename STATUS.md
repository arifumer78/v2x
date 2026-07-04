# Status

Honest snapshot of what's actually built vs. planned. Update this at the end of any work session — even a one-line touch is worth it.

**Last touched: 2026-07-04**

## Docs
- [x] Hitchhiker's Guide to ETSI C-V2X (`docs/hitchhikers-guide.md`)
- [x] PER Runtime design draft v0.1 (`docs/design/per-runtime-design.md`)
- [ ] Compiler design doc (not started — Phase 2)
- [ ] Open risks in design doc §9 verified against actual ETSI `.asn` modules (recursion, unbounded SIZE, SET/SET OF prevalence)

## Phase 1 — PER Runtime (`runtime/`)
- [ ] `BitCursor` (bit-position tracking, bounds-checked read/write)
- [ ] `Error` type / error taxonomy for `std::expected`
- [ ] `PerWriter`/`PerReader` — the seven primitives
  - [ ] `constrained_whole_number`
  - [ ] `semi_constrained_whole_number`
  - [ ] `unconstrained_whole_number`
  - [ ] `normally_small_length`
  - [ ] `length_determinant` (incl. 16K fragmentation)
  - [ ] `bits`
  - [ ] `bit`
  - [ ] `open_type`
- [ ] `Aligned` / `Unaligned` policy
- [ ] `Basic` / `Canonical` policy (lower priority — see design doc §5.2 scoping note)
- [ ] GTest unit tests (Layer 1: known-answer, boundary, fragmentation, zero-length)
- [ ] Property-based round-trip tests (Layer 2)
- [ ] Double round-trip tests (Layer 3)
- [ ] Differential tests vs. asn1c (Layer 4)
- [ ] Fuzz harness — libFuzzer/AFL++, ASan+UBSan (Layer 5)
- [ ] Branch coverage target reached on `PerReader`/`PerWriter`

## Phase 2 — Compiler (`compiler/`)
- [ ] Not started. Depends on Phase 1 being fuzz-stable first.

## Phase 3 — Tools (`tools/`)
- [ ] Not started.

## Phase 4 — Security (COER / 1609.2 / TS 103 097)
- [ ] Explicitly deferred, see design doc §8.
