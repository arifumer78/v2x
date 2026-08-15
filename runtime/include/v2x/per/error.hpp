#pragma once

#include <cstdint>

namespace v2x::per {

// uint8_t base: 6 small values, no reason to default to int (performance-enum-size).
enum class Error : std::uint8_t {
    BufferExhausted,   // read past end / write past capacity
    ValueOutOfRange,   // v < lb or v > ub
    LengthOutOfRange,  // length outside a given SizeRange constraint
    LengthTooLarge,    // exceeds an explicit sanity ceiling (bounded-execution posture)
    MalformedEncoding, // decode found an internally inconsistent state
    InvalidConstraint, // caller passed lb > ub
};

} // namespace v2x::per
