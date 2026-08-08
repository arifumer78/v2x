# antlr4 C++ runtime 4.13.2's ProfilingATNSimulator.cpp uses
# std::chrono::high_resolution_clock without including <chrono> directly,
# relying on transitive inclusion that does not hold under newer/stricter
# STL implementations (observed: MSVC 14.5x / VS 2026 toolset). Not yet
# fixed upstream as of 4.13.2 — https://github.com/antlr/antlr4.
# Runs with CWD set to the fetched source root (FetchContent PATCH_COMMAND
# convention). Idempotent: safe to re-run across reconfigures.
set(target_file "runtime/src/atn/ProfilingATNSimulator.cpp")

file(READ "${target_file}" content)
string(FIND "${content}" "#include <chrono>" already_patched)

if(already_patched EQUAL -1)
    string(REPLACE
        "#include \"atn/PredicateEvalInfo.h\""
        "#include <chrono>\n\n#include \"atn/PredicateEvalInfo.h\""
        content "${content}")
    file(WRITE "${target_file}" "${content}")
    message(STATUS "patch_antlr4_chrono: added missing #include <chrono> to ${target_file}")
else()
    message(STATUS "patch_antlr4_chrono: ${target_file} already patched, skipping")
endif()
