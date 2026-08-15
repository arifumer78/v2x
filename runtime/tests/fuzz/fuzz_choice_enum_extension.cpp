// libFuzzer harness (Layer 5): CHOICE selector + open_type unwrap, ENUMERATED, and
// SEQUENCE extension-addition decode paths against raw fuzzer-controlled bytes.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include "v2x/per/choice.hpp"
#include "v2x/per/enumerated.hpp"
#include "v2x/per/open_type_wrap.hpp"
#include "v2x/per/result.hpp"
#include "v2x/per/sequence_extension.hpp"
#include "v2x/per/uper.hpp"

using v2x::per::Status;
using v2x::per::UperReader;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    const auto buf = std::as_bytes(std::span<const uint8_t>(data, size));

    {
        UperReader r(buf);
        auto selector = v2x::per::read_choice_selector(r, 5);
        if (selector.ok()) {
            if (selector.value().is_extension) {
                std::array<std::byte, 128> scratch{};
                (void)v2x::per::read_open_type_wrapped(
                    r, scratch, [](auto &inner) { return inner.constrained_whole_number(0, 1000); });
            } else {
                (void)r.constrained_whole_number(0, 1000);
            }
        }
    }
    {
        UperReader r(buf);
        (void)v2x::per::read_enumerated(r, 10, /*extensible=*/true);
    }
    {
        UperReader r(buf);
        std::array<std::byte, 128> scratch{};
        (void)v2x::per::read_sequence_extension(r, /*known_count=*/2, scratch,
                                                 [](auto &inner, uint32_t) -> Status<> {
                                                     auto v = inner.constrained_whole_number(0, 1000);
                                                     if (!v) {
                                                         return Status<>::Err(v.error());
                                                     }
                                                     return Status<>::Ok();
                                                 });
    }

    return 0;
}
