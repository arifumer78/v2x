// libFuzzer harness (Layer 5): OCTET STRING / BIT STRING / SEQUENCE OF decode paths
// (root, constrained, and extensible variants) against raw fuzzer-controlled bytes.
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

#include "v2x/per/bit_string.hpp"
#include "v2x/per/error.hpp"
#include "v2x/per/octet_string.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/sequence_of.hpp"
#include "v2x/per/size_range.hpp"
#include "v2x/per/uper.hpp"

using v2x::per::Error;
using v2x::per::SizeRange;
using v2x::per::Status;
using v2x::per::UperReader;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    const auto buf = std::as_bytes(std::span<const uint8_t>(data, size));
    const SizeRange constraint{0, 100};

    {
        UperReader r(buf);
        std::array<std::byte, 256> out{};
        (void)v2x::per::read_octet_string(r, out);
    }
    {
        UperReader r(buf);
        std::array<std::byte, 256> out{};
        (void)v2x::per::read_octet_string(r, out, constraint);
    }
    {
        UperReader r(buf);
        std::array<std::byte, 256> out{};
        (void)v2x::per::read_extensible_octet_string(r, out, constraint);
    }
    {
        UperReader r(buf);
        std::array<std::byte, 256> out{};
        (void)v2x::per::read_bit_string(r, out, std::nullopt);
    }
    {
        UperReader r(buf);
        std::array<std::byte, 256> out{};
        (void)v2x::per::read_extensible_bit_string(r, out, constraint);
    }
    {
        UperReader r(buf);
        // decode_element MUST bounds-check idx itself (see sequence_of.hpp) — a
        // malicious declared count is exactly what this harness is stressing.
        std::array<int64_t, 64> elements{};
        (void)v2x::per::read_sequence_of(r, std::nullopt, [&](auto &reader, uint32_t idx) -> Status<> {
            auto v = reader.constrained_whole_number(0, 255);
            if (!v) {
                return Status<>::Err(v.error());
            }
            if (idx >= elements.size()) {
                return Status<>::Err(Error::BufferExhausted);
            }
            elements[idx] = v.value();
            return Status<>::Ok();
        });
    }

    return 0;
}
