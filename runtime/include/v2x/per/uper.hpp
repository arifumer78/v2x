#pragma once

#include "v2x/per/per_reader.hpp"
#include "v2x/per/per_writer.hpp"
#include "v2x/per/policy.hpp"

namespace v2x::per {

// UPER (Unaligned, Basic) is the ETSI ITS mandate and the only combination
// given full test rigor in this phase (see docs/design/per-runtime-design.md
// §5.2/§5.3, §11). The other three exist because the kernel is templated on
// policy rather than forked — genuinely zero extra code — but are not
// validated beyond compiling; see policy.hpp for exactly what is and isn't
// wired up for Aligned today.
using UperWriter = PerWriter<Unaligned, Basic>;
using UperReader = PerReader<Unaligned, Basic>;

using AperWriter = PerWriter<Aligned, Basic>;
using AperReader = PerReader<Aligned, Basic>;

using CuperWriter = PerWriter<Unaligned, Canonical>;
using CuperReader = PerReader<Unaligned, Canonical>;

using CperWriter = PerWriter<Aligned, Canonical>;
using CperReader = PerReader<Aligned, Canonical>;

} // namespace v2x::per
