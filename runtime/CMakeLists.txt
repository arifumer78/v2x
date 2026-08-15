option(V2X_ENABLE_ASAN "Build with AddressSanitizer (MSVC /fsanitize=address)" OFF)
option(V2X_ENABLE_UBSAN "Build with UndefinedBehaviorSanitizer (requires the ClangCL toolset — cmake -T ClangCL; not available under plain MSVC)" OFF)
option(V2X_ENABLE_FUZZ "Build libFuzzer harnesses under tests/fuzz (requires the ClangCL toolset — cmake -T ClangCL; not available under plain MSVC)" OFF)

if (MSVC AND V2X_ENABLE_ASAN)
    # Applied globally (not just to v2x_per) so GTest's own build is compiled
    # with matching instrumentation — MSVC's STL container annotations
    # (enabled under /fsanitize=address) are ABI-visible; mixing an
    # instrumented and non-instrumented build of the same STL types produces
    # LNK2038 annotation mismatches at link time.
    # /RTC (default in MSVC Debug) is incompatible with /fsanitize=address;
    # use a Release-family config (RelWithDebInfo/Release) for ASan builds.
    add_compile_options(/fsanitize=address)
endif()

if (V2X_ENABLE_UBSAN)
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "V2X_ENABLE_UBSAN requires the ClangCL toolset (cmake ... -T ClangCL) — UBSan is not available under plain cl.exe/MSVC.")
    endif()
    # -fno-sanitize-recover=undefined: abort on first UB found rather than
    # continuing (continuing after real UB risks the sanitizer itself
    # observing corrupted state) — matches ASan's always-abort default above.
    # Same applied-globally rationale as ASan: GTest must be built with
    # matching instrumentation to avoid ABI mismatches at link time.
    # No separate link flag: under the VS-generator ClangCL toolset, linking
    # goes through lld-link.exe directly (not via clang-cl.exe as a driver),
    # which doesn't understand "-fsanitize=undefined" as a link flag — but
    # doesn't need it either, since clang-cl already embeds an autolink
    # pragma for the UBSan runtime lib into each object file it compiles.
    add_compile_options(-fsanitize=undefined -fno-sanitize-recover=undefined)

    # clang-cl's bundled UBSan runtime (clang_rt.ubsan_standalone_cxx-*.lib)
    # is only built for the static CRT. Force it for the whole build — GTest
    # included, see gtest_force_shared_crt below — otherwise linking fails
    # with a RuntimeLibrary /failifmismatch error against that lib.
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if (V2X_ENABLE_FUZZ)
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "V2X_ENABLE_FUZZ requires the ClangCL toolset (cmake ... -T ClangCL) — libFuzzer is not available under plain cl.exe/MSVC.")
    endif()
    if (V2X_ENABLE_UBSAN)
        # Fuzz targets link clang-cl's *dynamic*-CRT ASan runtime (see
        # tests/fuzz/CMakeLists.txt) — incompatible with the static CRT
        # V2X_ENABLE_UBSAN forces above. Configure separately (a third build
        # directory, same as build-asan/build-ubsan already are).
        message(FATAL_ERROR "V2X_ENABLE_FUZZ and V2X_ENABLE_UBSAN can't be combined in one configure (conflicting CRT requirements) — use a separate build directory.")
    endif()
    if (MSVC AND V2X_ENABLE_ASAN)
        # MSVC's own /fsanitize=address (the ASan option above) and clang's
        # -fsanitize=fuzzer,address (tests/fuzz/CMakeLists.txt) are two
        # different ASan implementations/runtimes — don't mix them.
        message(FATAL_ERROR "V2X_ENABLE_FUZZ and V2X_ENABLE_ASAN can't be combined in one configure — use a separate build directory.")
    endif()

    # clang-cl's bundled clang_rt.fuzzer-x86_64.lib is static-CRT-only (found the hard
    # way: linking it against a /MD-built v2x_per fails with lld-link's
    # /failifmismatch RuntimeLibrary check). Force static for the whole build, same
    # "applied globally so GTest/v2x_per match" pattern as V2X_ENABLE_UBSAN above —
    # this option doesn't build GTest at all, but v2x_per itself must match.
    cmake_policy(SET CMP0091 NEW)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_library(v2x_per STATIC
    src/bit_cursor.cpp
    src/bit_ops.cpp
)
target_include_directories(v2x_per PUBLIC include)
set_target_properties(v2x_per PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
if (MSVC)
    target_compile_options(v2x_per PUBLIC /W4 /permissive-)
endif()

if (V2X_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    if (V2X_ENABLE_UBSAN OR V2X_ENABLE_FUZZ)
        # Must match v2x_per's forced static CRT above, or linking against
        # clang-cl's UBSan/fuzzer runtime libs fails with a RuntimeLibrary mismatch.
        set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
    else()
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests/unit)
    add_subdirectory(tests/property)
endif()

if (V2X_ENABLE_FUZZ)
    # Not gated on V2X_BUILD_TESTS: these are standalone libFuzzer executables (their
    # own main() comes from the fuzzer runtime), not GTest-based, not registered with
    # ctest — a genuinely separate build mode, not part of the regular test sweep.
    add_subdirectory(tests/fuzz)
endif()
