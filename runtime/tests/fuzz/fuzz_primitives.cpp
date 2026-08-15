// libFuzzer harness (Layer 5): raw fuzzer bytes as the wire buffer, fed through the
// core PerReader primitives directly. Only crash-freedom under ASan+UBSan-style
// instrumentation is the point here — correctness (decode(encode(x)) == x) is
// already covered by tests/unit (known-answer) and tests/property (randomized).
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "v2x/per/uper.hpp"

using v2x::per::UperReader;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    const auto buf = std::as_bytes(std::span<const uint8_t>(data, size));
    UperReader r(buf);

    (void)r.bit();
    std::array<std::byte, 8> scratch{};
    (void)r.bits(scratch, 37);
    (void)r.constrained_whole_number(0, 100);          // small fixed-width path
    (void)r.constrained_whole_number(-1'000'000, 1'000'000'000); // >65536 fallback path
    (void)r.semi_constrained_whole_number(-50);
    (void)r.unconstrained_whole_number();
    (void)r.normally_small_length();
    (void)r.length_determinant(std::nullopt);

    std::array<std::byte, 64> open_type_out{};
    (void)r.open_type(open_type_out);

    return 0;
}
