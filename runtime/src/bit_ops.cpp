#include "v2x/per/bit_ops.hpp"

namespace v2x::per {

void set_bit(std::span<std::byte> buf, size_t bit_pos, bool value) {
    const size_t byte_idx = bit_pos / 8;
    const size_t bit_in_byte = bit_pos % 8;
    const std::byte mask = std::byte{1} << (7 - bit_in_byte);
    if (value) {
        buf[byte_idx] |= mask;
    } else {
        buf[byte_idx] &= ~mask;
    }
}

bool get_bit(std::span<const std::byte> buf, size_t bit_pos) {
    const size_t byte_idx = bit_pos / 8;
    const size_t bit_in_byte = bit_pos % 8;
    const std::byte mask = std::byte{1} << (7 - bit_in_byte);
    return (buf[byte_idx] & mask) != std::byte{0};
}

void write_bits(std::span<std::byte> dst, size_t bit_pos, std::span<const std::byte> src, size_t n_bits) {
    for (size_t i = 0; i < n_bits; ++i) {
        set_bit(dst, bit_pos + i, get_bit(src, i));
    }
}

void read_bits(std::span<const std::byte> src, size_t bit_pos, std::span<std::byte> dst, size_t n_bits) {
    for (size_t i = 0; i < n_bits; ++i) {
        set_bit(dst, i, get_bit(src, bit_pos + i));
    }
}

} // namespace v2x::per
