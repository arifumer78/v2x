#pragma once

// Test-only helpers for property-based (Layer 2) and double-round-trip (Layer 3)
// tests. Not shipped — nothing in runtime/include depends on this.
//
// No external property-testing library (e.g. RapidCheck) is used: matches this
// project's existing hand-rolled-over-dependency pattern (Result<T,E> instead of
// std::expected, etc.), and no new tooling was wanted for this round. In place of a
// real framework's shrinking-on-failure, a fixed default seed — overridable via the
// V2X_PROPERTY_SEED environment variable, printed once at the start of a test run —
// makes any failure reproducible by rerunning with that seed.

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace v2x::per::test {

inline uint64_t PropertySeed() {
    static const uint64_t seed = [] {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // getenv: fine here — single-threaded test startup, read-only
#endif
        const char* env = std::getenv("V2X_PROPERTY_SEED");
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        if (env != nullptr) {
            return static_cast<uint64_t>(std::strtoull(env, nullptr, 10));
        }
        return uint64_t{0xA5A5'1234'C0FF'EEULL}; // fixed default: reproducible unless overridden
    }();
    static const bool printed = [] {
        std::cerr << "[property] V2X_PROPERTY_SEED=" << seed
                   << " (rerun with this env var set to reproduce a failure)\n";
        return true;
    }();
    (void)printed;
    return seed;
}

inline std::mt19937_64& Rng() {
    static std::mt19937_64 rng(PropertySeed());
    return rng;
}

inline int64_t RandomInRange(std::mt19937_64& rng, int64_t lb, int64_t ub) {
    std::uniform_int_distribution<int64_t> dist(lb, ub);
    return dist(rng);
}

inline std::vector<std::byte> RandomBytes(std::mt19937_64& rng, size_t n) {
    std::uniform_int_distribution<int> dist(0, 255);
    std::vector<std::byte> out(n);
    for (auto& b : out) {
        b = static_cast<std::byte>(dist(rng));
    }
    return out;
}

// Biased around `boundary` (the 16384-unit PER fragmentation threshold) rather than
// uniform over some large range — that's the highest-risk region per
// per_writer.hpp/per_reader.hpp's own fragmentation-loop comments, and a purely
// uniform generator over a huge range would almost never land near it.
inline uint32_t RandomCountNearBoundary(std::mt19937_64& rng, uint32_t boundary = 16384, uint32_t spread = 200) {
    const int64_t delta = RandomInRange(rng, -static_cast<int64_t>(spread), static_cast<int64_t>(spread));
    const int64_t value = static_cast<int64_t>(boundary) + delta;
    return static_cast<uint32_t>(value < 0 ? 0 : value);
}

} // namespace v2x::per::test
