#pragma once

#include <cstdint>
#include <optional>

namespace v2x::per {

struct SizeRange {
    uint32_t min;
    std::optional<uint32_t> max; // nullopt = unbounded/general length determinant
};

} // namespace v2x::per
