#pragma once

#include <cstddef>
#include <span>

namespace v2x::per {

// Bit numbering: bit 0 is the most-significant bit of byte 0 (PER convention —
// the first bit transmitted is the MSB of the first octet). Bounds are the
// caller's responsibility; BitCursor::advance already validated the range
// before any of these are called.

void set_bit(std::span<std::byte> buf, size_t bit_pos, bool value);
bool get_bit(std::span<const std::byte> buf, size_t bit_pos);

// Bit-at-a-time, deliberately not shift-optimized. ETSI ITS message sizes are
// small enough that this trades a little speed for a much smaller bug surface
// in a from-scratch, not-yet-fuzz-hardened kernel.
void write_bits(std::span<std::byte> dst, size_t bit_pos, std::span<const std::byte> src, size_t n_bits);
void read_bits(std::span<const std::byte> src, size_t bit_pos, std::span<std::byte> dst, size_t n_bits);

} // namespace v2x::per
