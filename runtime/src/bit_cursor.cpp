#include "v2x/per/bit_cursor.hpp"

namespace v2x::per {

BitCursor::BitCursor(size_t total_bits) : total_bits_(total_bits) {}

size_t BitCursor::position() const { return pos_; }

size_t BitCursor::total_bits() const { return total_bits_; }

size_t BitCursor::remaining_bits() const { return total_bits_ - pos_; }

bool BitCursor::advance(size_t n_bits) {
    if (n_bits > remaining_bits()) {
        return false;
    }
    pos_ += n_bits;
    return true;
}

void BitCursor::pad_to_octet() {
    const size_t remainder = pos_ % 8;
    if (remainder == 0) {
        return;
    }
    const size_t pad = 8 - remainder;
    const size_t new_pos = pos_ + pad;
    pos_ = (new_pos <= total_bits_) ? new_pos : total_bits_;
}

} // namespace v2x::per
