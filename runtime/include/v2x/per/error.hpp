#pragma once

namespace v2x::per {

enum class Error {
    BufferExhausted,   // read past end / write past capacity
    ValueOutOfRange,   // v < lb or v > ub
    LengthOutOfRange,  // length outside a given SizeRange constraint
    LengthTooLarge,    // exceeds an explicit sanity ceiling (bounded-execution posture)
    MalformedEncoding, // decode found an internally inconsistent state
    InvalidConstraint, // caller passed lb > ub
};

} // namespace v2x::per
