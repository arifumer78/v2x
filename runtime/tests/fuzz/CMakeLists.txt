# libFuzzer harnesses (Layer 5) — see docs/design/per-runtime-design.md and
# STATUS.md's Phase 1 section for how this build recipe was derived. Two things
# don't work the way UBSan's do:
#
# 1. Raw clang-cl on the command line can't link libFuzzer targets on a VS-bundled
#    clang install — Microsoft's compiler-rt fork has private __coe_win::* hooks with
#    no public .lib to satisfy them. Going through this CMake -> MSBuild path (the
#    ClangCL platform toolset) avoids that; raw clang-cl does not.
# 2. Unlike UBSan's runtime, clang-cl does not autolink the fuzzer runtime lib via a
#    per-object embedded pragma — it must be linked explicitly, found here by walking
#    from the compiler binary rather than hardcoding an LLVM version number (that
#    directory segment, e.g. ".../lib/clang/22/lib/windows/", changes across VS/LLVM
#    updates).
get_filename_component(V2X_CLANG_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
file(GLOB V2X_CLANG_RT_WINDOWS_DIRS "${V2X_CLANG_BIN_DIR}/../lib/clang/*/lib/windows")
list(LENGTH V2X_CLANG_RT_WINDOWS_DIRS V2X_CLANG_RT_DIR_COUNT)
if (V2X_CLANG_RT_DIR_COUNT EQUAL 0)
    message(FATAL_ERROR "V2X_ENABLE_FUZZ: could not locate clang-cl's bundled sanitizer runtime libs under ${V2X_CLANG_BIN_DIR}/../lib/clang/*/lib/windows")
endif()
list(GET V2X_CLANG_RT_WINDOWS_DIRS 0 V2X_CLANG_RT_WINDOWS_DIR)

set(V2X_FUZZ_RUNTIME_LIBS
    "${V2X_CLANG_RT_WINDOWS_DIR}/clang_rt.fuzzer-x86_64.lib"
    "${V2X_CLANG_RT_WINDOWS_DIR}/clang_rt.asan_dynamic-x86_64.lib"
    "${V2X_CLANG_RT_WINDOWS_DIR}/clang_rt.asan_static_runtime_thunk-x86_64.lib"
    dbghelp.lib
)

# -fsanitize=fuzzer,address: coverage-guided fuzzing plus memory-safety checking on
# every run, not just a fixed corpus — the actual point of Layer 5 versus the
# already-existing ASan-over-Layer-1 check. clang_rt.fuzzer-x86_64.lib is
# static-CRT-only (../../CMakeLists.txt forces CMAKE_MSVC_RUNTIME_LIBRARY to static
# for the whole V2X_ENABLE_FUZZ build accordingly) — but this LLVM distribution only
# ships a *dynamic*-CRT ASan main lib, no static one. The "static_runtime_thunk"
# variant (not "dynamic_runtime_thunk") is exactly clang-cl's documented shim for
# that combination: a static-CRT executable using the dynamic ASan DLL.
function(v2x_add_fuzz_target name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE v2x_per ${V2X_FUZZ_RUNTIME_LIBS})
    target_compile_options(${name} PRIVATE -fsanitize=fuzzer,address)
    # -fsanitize=address auto-enables MSVC STL's container-annotation feature
    # (bounds-tagging std::string/std::vector internals for extra ASan checks) —
    # ABI-visible, so every linked object must agree on it or lld-link's
    # /failifmismatch rejects the link. v2x_per and clang_rt.fuzzer-x86_64.lib
    # weren't built with it, and neither library actually uses std::string/vector in
    # the fuzzed code paths (span/array throughout) — so it's disabled here rather
    # than chasing annotation-matching across a prebuilt runtime lib we can't
    # recompile. Core ASan (heap/global/stack redzones) is unaffected either way.
    target_compile_definitions(${name} PRIVATE _DISABLE_STRING_ANNOTATION _DISABLE_VECTOR_ANNOTATION)
    set_target_properties(${name} PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
endfunction()

# Each harness feeds raw fuzzer bytes directly as the wire buffer to PerReader/the
# orchestration-layer read_* functions — the actual untrusted-input boundary (encoding
# is always host-controlled from known-good values; decoding is what a real ECU does
# to bytes off the wire). Grouped by area rather than one harness per primitive, same
# "quick win, not exhaustive" granularity as tests/property/.
v2x_add_fuzz_target(fuzz_primitives fuzz_primitives.cpp)
v2x_add_fuzz_target(fuzz_strings_and_sequence fuzz_strings_and_sequence.cpp)
v2x_add_fuzz_target(fuzz_choice_enum_extension fuzz_choice_enum_extension.cpp)
