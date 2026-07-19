#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>

#include "v2x/per/bit_cursor.hpp"
#include "v2x/per/bit_ops.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/policy.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/size_range.hpp"

namespace v2x::per {

template <typename AlignPolicy, typename CanonicalPolicy = Basic>
class PerWriter;

// Shared by open_type (unit_bits=8, no constraint) and, eventually,
// octet_string (unit_bits=8, optional SizeRange) / bit_string (unit_bits=1,
// optional SizeRange) — see per_writer.hpp's open_type comment and
// docs/design/per-runtime-design.md §4.3. `content`/`total_bits` describe the
// full payload to be length-prefixed and (if total_bits/unit_bits >= 16384)
// fragmented; declared here (forward), defined after PerWriter below.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_length_prefixed_content(PerWriter<AlignPolicy, CanonicalPolicy>& w,
                                        std::span<const std::byte> content, size_t total_bits,
                                        uint32_t unit_bits, std::optional<SizeRange> constraint);

template <typename AlignPolicy, typename CanonicalPolicy>
class PerWriter {
public:
    explicit PerWriter(std::span<std::byte> out_buffer)
        : buffer_(out_buffer), cursor_(out_buffer.size() * 8) {
        // Padding/skipped bits (alignment, unused trailing bits after finish())
        // must read back as zero without bit_ops needing to write them explicitly.
        for (std::byte& b : buffer_) {
            b = std::byte{0};
        }
    }

    Status<> bit(bool value) {
        const size_t pos = cursor_.position();
        if (!cursor_.advance(1)) {
            return Status<>::Err(Error::BufferExhausted);
        }
        set_bit(buffer_, pos, value);
        return Status<>::Ok();
    }

    Status<> bits(std::span<const std::byte> raw, size_t n_bits) {
        if (n_bits == 0) {
            return Status<>::Ok();
        }
        const size_t pos = cursor_.position();
        if (!cursor_.advance(n_bits)) {
            return Status<>::Err(Error::BufferExhausted);
        }
        write_bits(buffer_, pos, raw, n_bits);
        return Status<>::Ok();
    }

    // X.691 §10.5. range==1 (lb==ub) encodes as zero bits. range<=65536 (i.e. a
    // bit width of at most 16) packs (v-lb) as that many unaligned bits, MSB
    // first — no octet padding in UPER, unlike APER. range>65536 falls back to
    // a length-prefixed minimal-octet unsigned encoding of (v-lb), per §10.5.7.4.
    Status<> constrained_whole_number(int64_t v, int64_t lb, int64_t ub) {
        if (lb > ub) {
            return Status<>::Err(Error::InvalidConstraint);
        }
        if (v < lb || v > ub) {
            return Status<>::Err(Error::ValueOutOfRange);
        }
        // Computed via uint64_t subtraction (well-defined wraparound) rather than
        // int64_t (ub-lb can overflow int64_t, e.g. lb=INT64_MIN, ub=INT64_MAX).
        const uint64_t span = static_cast<uint64_t>(ub) - static_cast<uint64_t>(lb);
        const uint64_t offset = static_cast<uint64_t>(v) - static_cast<uint64_t>(lb);
        if (span == 0) {
            return Status<>::Ok(); // single possible value — zero bits
        }
        const int bit_width = std::bit_width(span);
        if (bit_width <= 16) {
            return write_fixed_width_bits_(offset, static_cast<size_t>(bit_width));
        }
        return write_unsigned_min_octets_(offset);
    }

    // X.691 §10.7. offset = v - lb (>= 0), encoded as a length-prefixed
    // minimal-octet unsigned value — the general (unbounded-above) case of the
    // fixed-width/fallback split constrained_whole_number makes for range>65536.
    Status<> semi_constrained_whole_number(int64_t v, int64_t lb) {
        if (v < lb) {
            return Status<>::Err(Error::ValueOutOfRange);
        }
        const uint64_t offset = static_cast<uint64_t>(v) - static_cast<uint64_t>(lb);
        return write_unsigned_min_octets_(offset);
    }

    // X.691 §10.8. Minimal two's-complement octets of v, length-prefixed.
    Status<> unconstrained_whole_number(int64_t v) { return write_signed_min_octets_(v); }

    // X.691 §10.9. If constraint bounds n to a known range <=65535, delegates
    // entirely to constrained_whole_number(n, min, max) — no marker/length
    // mechanics at all in that case (a fixed-width field is enough). Otherwise:
    // n<128 -> single byte, top bit 0; 128<=n<=16383 -> two bytes, top bits
    // '10'; n>=16384 -> one fragment marker (top bits '11', low 6 bits = a
    // 1-4 count of 16384-unit chunks). UPER never pre-pads to an octet
    // boundary before any of this, unlike APER.
    //
    // Real X.691 fragmentation interleaves markers with the actual content
    // being length-prefixed (marker, 16K content units, next marker, ...) —
    // this primitive only knows n, not the content, so it can only commit to
    // ONE header per call. The return value is the number of the n units that
    // header covers: always n, except for a fragment marker, where it's the
    // chunk size (a multiple of 16384) — the caller must then write exactly
    // that many content units and call this again with the remaining count.
    Result<uint32_t> length_determinant(uint32_t n, std::optional<SizeRange> constraint) {
        if (constraint && constraint->max.has_value() && *constraint->max <= 65535) {
            if (n < constraint->min || n > *constraint->max) {
                return Result<uint32_t>::Err(Error::LengthOutOfRange);
            }
            auto status = constrained_whole_number(static_cast<int64_t>(n), static_cast<int64_t>(constraint->min),
                                                     static_cast<int64_t>(*constraint->max));
            if (!status) {
                return Result<uint32_t>::Err(status.error());
            }
            return Result<uint32_t>::Ok(n);
        }
        if (n < 128) {
            const std::byte b = static_cast<std::byte>(n & 0x7Fu); // top bit 0
            auto status = bits(std::span<const std::byte>(&b, 1), 8);
            if (!status) {
                return Result<uint32_t>::Err(status.error());
            }
            return Result<uint32_t>::Ok(n);
        }
        if (n <= 16383) {
            const auto v = static_cast<uint16_t>(0x8000u | n); // top two bits '10'
            const std::array<std::byte, 2> bytes{static_cast<std::byte>((v >> 8) & 0xFFu),
                                                  static_cast<std::byte>(v & 0xFFu)};
            auto status = bits(bytes, 16);
            if (!status) {
                return Result<uint32_t>::Err(status.error());
            }
            return Result<uint32_t>::Ok(n);
        }
        uint32_t k = n / 16384;
        if (k > 4) {
            k = 4;
        }
        const uint32_t chunk = k * 16384;
        const std::byte marker = static_cast<std::byte>(0xC0u | (k & 0x3Fu)); // top two bits '11'
        auto status = bits(std::span<const std::byte>(&marker, 1), 8);
        if (!status) {
            return Result<uint32_t>::Err(status.error());
        }
        return Result<uint32_t>::Ok(chunk);
    }

    // X.691 §11.6, verified against the actual spec text (an earlier version
    // of this had the threshold, the small-form value, and the large-form
    // shape all wrong — see docs/CHANGELOG.md). n<=63: flag bit 0 + n itself
    // (not n-1) as a 6-bit field. n>=64: flag bit 1 + n encoded as a
    // semi-constrained whole number with lb=0 — i.e. the same
    // length-of-octet-count-prefixed minimal-octets shape as
    // semi_constrained_whole_number/write_unsigned_min_octets_ below, NOT a
    // general length_determinant call (which would treat n as a length
    // value with its own short/medium/fragmentation forms — a different,
    // spec-incorrect encoding). This shape has no upper limit on n, unlike
    // the earlier (wrong) implementation which rejected large values.
    Status<> normally_small_length(uint32_t n) {
        if (n <= 63) {
            auto flag = bit(false);
            if (!flag) {
                return flag;
            }
            return write_fixed_width_bits_(static_cast<uint64_t>(n), 6);
        }
        auto flag = bit(true);
        if (!flag) {
            return flag;
        }
        return write_unsigned_min_octets_(static_cast<uint64_t>(n));
    }

    // Extension-addition wrapping / unknown-CHOICE-alternative fallback:
    // length-determinant-prefixed encapsulation of an already-encoded buffer.
    // Unlike normally_small_length, open_type has real content to interleave
    // with fragment markers, so it drives the actual multi-call fragmentation
    // loop itself — see write_length_prefixed_content below (shared with
    // octet_string/bit_string, which are the same shape with a SizeRange and,
    // for bit_string, a bit- rather than octet-counted length).
    Status<> open_type(std::span<const std::byte> encoded) {
        return write_length_prefixed_content(*this, encoded, encoded.size() * 8, 8, std::nullopt);
    }

    [[nodiscard]] size_t bits_written() const { return cursor_.position(); }

    std::span<const std::byte> finish() {
        AlignPolicy::align(cursor_);
        return buffer_.subspan(0, (cursor_.position() + 7) / 8);
    }

private:
    // Packs the low n_bits of value as n_bits unaligned bits, MSB first.
    Status<> write_fixed_width_bits_(uint64_t value, size_t n_bits) {
        for (size_t i = n_bits; i-- > 0;) {
            const bool b = ((value >> i) & 1u) != 0;
            auto status = bit(b);
            if (!status) {
                return status;
            }
        }
        return Status<>::Ok();
    }

    // Length prefix shared by all three min-octets helpers below — deliberately
    // the length_determinant short form only (single byte, top bit 0, low 7
    // bits = octet count). n is at most 8 here (octet count of a 64-bit value),
    // which can never trigger the medium or fragmentation forms. The general
    // length_determinant primitive (with those forms) lands separately.
    Status<> write_short_length_prefix_(uint32_t n) {
        const std::byte len_byte = static_cast<std::byte>(n & 0x7Fu);
        return bits(std::span<const std::byte>(&len_byte, 1), 8);
    }

    // Length-prefixed minimal-octet unsigned encoding of value. Shared by
    // constrained_whole_number's fallback path and semi_constrained_whole_number.
    Status<> write_unsigned_min_octets_(uint64_t value) {
        uint32_t n = 1;
        if (value != 0) {
            n = static_cast<uint32_t>((std::bit_width(value) + 7) / 8);
        }
        auto len_status = write_short_length_prefix_(n);
        if (!len_status) {
            return len_status;
        }
        std::array<std::byte, 8> octets{};
        for (uint32_t i = 0; i < 8; ++i) {
            octets[7 - i] = static_cast<std::byte>((value >> (i * 8)) & 0xFFu);
        }
        const std::span<const std::byte> src(octets.data() + (8 - n), n);
        return bits(src, n * 8);
    }

    // Length-prefixed minimal two's-complement octet encoding of v. n is the
    // smallest octet count such that v fits in n*8-bit signed two's complement;
    // computed via the bitwise-complement-of-magnitude trick so it's correct
    // (and overflow-free) for the full int64_t range including INT64_MIN.
    Status<> write_signed_min_octets_(int64_t v) {
        const uint64_t comp = (v >= 0) ? static_cast<uint64_t>(v) : ~static_cast<uint64_t>(v);
        const int bits_needed = std::bit_width(comp) + 1; // +1 for the sign bit
        const uint32_t n = static_cast<uint32_t>((bits_needed + 7) / 8);
        auto len_status = write_short_length_prefix_(n);
        if (!len_status) {
            return len_status;
        }
        const uint64_t bits_pattern = static_cast<uint64_t>(v);
        std::array<std::byte, 8> octets{};
        for (uint32_t i = 0; i < 8; ++i) {
            octets[7 - i] = static_cast<std::byte>((bits_pattern >> (i * 8)) & 0xFFu);
        }
        const std::span<const std::byte> src(octets.data() + (8 - n), n);
        return bits(src, n * 8);
    }

    std::span<std::byte> buffer_;
    BitCursor cursor_;
};

// total_bits must be an exact multiple of unit_bits (the caller's
// responsibility — e.g. octet-counted content is always byte-aligned by
// construction). Fragmentation chunks (multiples of 16384 units) are always
// byte-aligned in bits regardless of unit_bits, since 16384 is itself a
// multiple of 8 — so content can always be sliced at byte offsets between
// iterations; only the final (non-fragmenting) transfer can have a
// non-multiple-of-8 bit count, which bits() already supports directly.
template <typename AlignPolicy, typename CanonicalPolicy>
Status<> write_length_prefixed_content(PerWriter<AlignPolicy, CanonicalPolicy>& w,
                                        std::span<const std::byte> content, size_t total_bits,
                                        uint32_t unit_bits, std::optional<SizeRange> constraint) {
    size_t bit_offset = 0;
    while (true) {
        const size_t remaining_units = (total_bits - bit_offset) / unit_bits;
        if (remaining_units > std::numeric_limits<uint32_t>::max()) {
            return Status<>::Err(Error::LengthTooLarge);
        }
        auto ld = w.length_determinant(static_cast<uint32_t>(remaining_units), constraint);
        if (!ld) {
            return Status<>::Err(ld.error());
        }
        const uint32_t chunk_units = ld.value();
        const size_t chunk_bits = static_cast<size_t>(chunk_units) * unit_bits;
        auto content_status = w.bits(content.subspan(bit_offset / 8), chunk_bits);
        if (!content_status) {
            return content_status;
        }
        bit_offset += chunk_bits;
        if (chunk_units < 16384) {
            break;
        }
    }
    return Status<>::Ok();
}

} // namespace v2x::per
