#pragma once

#include <cstddef>

namespace v2x::per {

// Pure bit-position arithmetic — owns no buffer. Kept trivially unit-testable in
// isolation and shared by both PerWriter and PerReader, matching the alignment
// policy hook signature (Aligned::align(BitCursor&) / Unaligned::align(BitCursor&)).
class BitCursor {
public:
    explicit BitCursor(size_t total_bits);

    [[nodiscard]] size_t position() const;
    [[nodiscard]] size_t total_bits() const;
    [[nodiscard]] size_t remaining_bits() const;

    // Advances by n_bits. Returns false (no-op, position unchanged) if that would
    // exceed total_bits().
    [[nodiscard]] bool advance(size_t n_bits);

    // Advances to the next multiple-of-8 position. No-op if already aligned.
    // Saturates at total_bits() if the buffer is too short to reach the next
    // octet boundary, preserving the pos_ <= total_bits_ invariant.
    void pad_to_octet();

private:
    size_t total_bits_;
    size_t pos_ = 0;
};

} // namespace v2x::per
