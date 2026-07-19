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
class PerReader;

// Mirrors PerWriter::write_length_prefixed_content. Returns the total number
// of BITS decoded into out (not bytes — bit_string needs the exact bit count
// for a non-byte-aligned final chunk; open_type/octet_string divide by 8,
// which is exact for them since unit_bits=8 keeps every chunk byte-aligned).
template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_length_prefixed_content(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                             uint32_t unit_bits, std::optional<SizeRange> constraint);

template <typename AlignPolicy, typename CanonicalPolicy>
class PerReader {
public:
    explicit PerReader(std::span<const std::byte> in_buffer)
        : buffer_(in_buffer), cursor_(in_buffer.size() * 8) {}

    Result<bool> bit() {
        const size_t pos = cursor_.position();
        if (!cursor_.advance(1)) {
            return Result<bool>::Err(Error::BufferExhausted);
        }
        return Result<bool>::Ok(get_bit(buffer_, pos));
    }

    Status<> bits(std::span<std::byte> out, size_t n_bits) {
        if (n_bits == 0) {
            return Status<>::Ok();
        }
        const size_t pos = cursor_.position();
        if (!cursor_.advance(n_bits)) {
            return Status<>::Err(Error::BufferExhausted);
        }
        read_bits(buffer_, pos, out, n_bits);
        return Status<>::Ok();
    }

    // Mirrors PerWriter::constrained_whole_number — see its comment for the
    // X.691 §10.5 rule and the fixed-width / fallback boundary.
    Result<int64_t> constrained_whole_number(int64_t lb, int64_t ub) {
        if (lb > ub) {
            return Result<int64_t>::Err(Error::InvalidConstraint);
        }
        const uint64_t span = static_cast<uint64_t>(ub) - static_cast<uint64_t>(lb);
        if (span == 0) {
            return Result<int64_t>::Ok(lb);
        }
        const int bit_width = std::bit_width(span);
        uint64_t offset = 0;
        if (bit_width <= 16) {
            auto r = read_fixed_width_bits_(static_cast<size_t>(bit_width));
            if (!r) {
                return Result<int64_t>::Err(r.error());
            }
            offset = r.value();
        } else {
            auto r = read_unsigned_min_octets_();
            if (!r) {
                return Result<int64_t>::Err(r.error());
            }
            offset = r.value();
        }
        if (offset > span) {
            // Decoded offset can't correspond to any value in [lb, ub] — malformed input.
            return Result<int64_t>::Err(Error::MalformedEncoding);
        }
        return Result<int64_t>::Ok(static_cast<int64_t>(static_cast<uint64_t>(lb) + offset));
    }

    // Mirrors PerWriter::semi_constrained_whole_number.
    Result<int64_t> semi_constrained_whole_number(int64_t lb) {
        auto r = read_unsigned_min_octets_();
        if (!r) {
            return Result<int64_t>::Err(r.error());
        }
        return Result<int64_t>::Ok(static_cast<int64_t>(static_cast<uint64_t>(lb) + r.value()));
    }

    // Mirrors PerWriter::unconstrained_whole_number.
    Result<int64_t> unconstrained_whole_number() { return read_signed_min_octets_(); }

    // Mirrors PerWriter::length_determinant — see its comment for the form
    // selection and the "chunk size, not always n" return-value contract.
    Result<uint32_t> length_determinant(std::optional<SizeRange> constraint) {
        if (constraint && constraint->max.has_value() && *constraint->max <= 65535) {
            auto r = constrained_whole_number(static_cast<int64_t>(constraint->min),
                                               static_cast<int64_t>(*constraint->max));
            if (!r) {
                return Result<uint32_t>::Err(r.error());
            }
            return Result<uint32_t>::Ok(static_cast<uint32_t>(r.value()));
        }
        auto b0 = bit();
        if (!b0) {
            return Result<uint32_t>::Err(b0.error());
        }
        if (!b0.value()) {
            auto rest = read_fixed_width_bits_(7);
            if (!rest) {
                return Result<uint32_t>::Err(rest.error());
            }
            return Result<uint32_t>::Ok(static_cast<uint32_t>(rest.value()));
        }
        auto b1 = bit();
        if (!b1) {
            return Result<uint32_t>::Err(b1.error());
        }
        if (!b1.value()) {
            auto rest = read_fixed_width_bits_(14);
            if (!rest) {
                return Result<uint32_t>::Err(rest.error());
            }
            return Result<uint32_t>::Ok(static_cast<uint32_t>(rest.value()));
        }
        auto rest = read_fixed_width_bits_(6);
        if (!rest) {
            return Result<uint32_t>::Err(rest.error());
        }
        const uint64_t k = rest.value();
        if (k < 1 || k > 4) {
            return Result<uint32_t>::Err(Error::MalformedEncoding);
        }
        return Result<uint32_t>::Ok(static_cast<uint32_t>(k * 16384));
    }

    // Mirrors PerWriter::normally_small_length — see its comment for the
    // X.691 §11.6 shape and the bug that was fixed here after cross-checking
    // against the actual spec text.
    Result<uint32_t> normally_small_length() {
        auto flag = bit();
        if (!flag) {
            return Result<uint32_t>::Err(flag.error());
        }
        if (!flag.value()) {
            auto r = read_fixed_width_bits_(6);
            if (!r) {
                return Result<uint32_t>::Err(r.error());
            }
            return Result<uint32_t>::Ok(static_cast<uint32_t>(r.value()));
        }
        auto r = read_unsigned_min_octets_();
        if (!r) {
            return Result<uint32_t>::Err(r.error());
        }
        if (r.value() > std::numeric_limits<uint32_t>::max()) {
            return Result<uint32_t>::Err(Error::MalformedEncoding);
        }
        return Result<uint32_t>::Ok(static_cast<uint32_t>(r.value()));
    }

    // Mirrors PerWriter::open_type. Deviates from the design doc's original
    // parameterless span-returning sketch: fragmented content isn't contiguous
    // in the wire buffer (length markers sit between chunks), so reassembly
    // needs a caller-provided destination — mirrors bits()'s out-parameter
    // pattern rather than allocating. Returns the number of bytes written
    // into out (the decoded, defragmented content); out must be at least that
    // large or this fails with BufferExhausted before overrunning it.
    Result<size_t> open_type(std::span<std::byte> out) {
        auto r = read_length_prefixed_content(*this, out, 8, std::nullopt);
        if (!r) {
            return r;
        }
        return Result<size_t>::Ok(r.value() / 8); // exact: unit_bits=8 keeps every chunk byte-aligned
    }

    [[nodiscard]] size_t bits_remaining() const { return cursor_.remaining_bits(); }

private:
    Result<uint64_t> read_fixed_width_bits_(size_t n_bits) {
        uint64_t value = 0;
        for (size_t i = 0; i < n_bits; ++i) {
            auto b = bit();
            if (!b) {
                return Result<uint64_t>::Err(b.error());
            }
            value = (value << 1) | (b.value() ? 1u : 0u);
        }
        return Result<uint64_t>::Ok(value);
    }

    // Mirrors PerWriter::write_short_length_prefix_ — see its comment for why
    // this is deliberately short-form-only.
    Result<uint32_t> read_short_length_prefix_() {
        std::array<std::byte, 1> len_byte{};
        auto ls = bits(len_byte, 8);
        if (!ls) {
            return Result<uint32_t>::Err(ls.error());
        }
        const auto raw = static_cast<uint8_t>(len_byte[0]);
        if ((raw & 0x80u) != 0) {
            return Result<uint32_t>::Err(Error::MalformedEncoding);
        }
        const uint32_t n = raw & 0x7Fu;
        if (n > 8) {
            return Result<uint32_t>::Err(Error::MalformedEncoding);
        }
        return Result<uint32_t>::Ok(n);
    }

    Result<uint64_t> read_unsigned_min_octets_() {
        auto ln = read_short_length_prefix_();
        if (!ln) {
            return Result<uint64_t>::Err(ln.error());
        }
        const uint32_t n = ln.value();
        std::array<std::byte, 8> octets{};
        auto bs = bits(std::span<std::byte>(octets.data(), n), n * 8);
        if (!bs) {
            return Result<uint64_t>::Err(bs.error());
        }
        uint64_t value = 0;
        for (uint32_t i = 0; i < n; ++i) {
            value = (value << 8) | static_cast<uint64_t>(octets[i]);
        }
        return Result<uint64_t>::Ok(value);
    }

    // Mirrors PerWriter::write_signed_min_octets_. Reads n octets and
    // sign-extends from n*8 bits to 64 bits.
    Result<int64_t> read_signed_min_octets_() {
        auto ln = read_short_length_prefix_();
        if (!ln) {
            return Result<int64_t>::Err(ln.error());
        }
        const uint32_t n = ln.value();
        std::array<std::byte, 8> octets{};
        auto bs = bits(std::span<std::byte>(octets.data(), n), n * 8);
        if (!bs) {
            return Result<int64_t>::Err(bs.error());
        }
        if (n == 0) {
            return Result<int64_t>::Ok(0); // defensive; the writer never emits n=0
        }
        uint64_t value = 0;
        for (uint32_t i = 0; i < n; ++i) {
            value = (value << 8) | static_cast<uint64_t>(octets[i]);
        }
        const uint32_t bits_read = n * 8;
        if (bits_read < 64 && (value & (uint64_t{1} << (bits_read - 1))) != 0) {
            value |= (~uint64_t{0} << bits_read);
        }
        return Result<int64_t>::Ok(static_cast<int64_t>(value));
    }

    std::span<const std::byte> buffer_;
    BitCursor cursor_;
};

// See PerWriter::write_length_prefixed_content for the byte-alignment
// argument this relies on (fragmentation chunks are always a multiple of
// 16384 units, hence always byte-aligned in bits regardless of unit_bits).
template <typename AlignPolicy, typename CanonicalPolicy>
Result<size_t> read_length_prefixed_content(PerReader<AlignPolicy, CanonicalPolicy>& r, std::span<std::byte> out,
                                             uint32_t unit_bits, std::optional<SizeRange> constraint) {
    size_t bit_offset = 0;
    while (true) {
        auto ld = r.length_determinant(constraint);
        if (!ld) {
            return Result<size_t>::Err(ld.error());
        }
        const uint32_t chunk_units = ld.value();
        const size_t chunk_bits = static_cast<size_t>(chunk_units) * unit_bits;
        const size_t byte_offset = bit_offset / 8;
        const size_t chunk_bytes_needed = (chunk_bits + 7) / 8;
        if (byte_offset + chunk_bytes_needed > out.size()) {
            return Result<size_t>::Err(Error::BufferExhausted);
        }
        auto content_status = r.bits(out.subspan(byte_offset), chunk_bits);
        if (!content_status) {
            return Result<size_t>::Err(content_status.error());
        }
        bit_offset += chunk_bits;
        if (chunk_units < 16384) {
            break;
        }
    }
    return Result<size_t>::Ok(bit_offset);
}

} // namespace v2x::per
