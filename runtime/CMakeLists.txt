option(V2X_ENABLE_ASAN "Build with AddressSanitizer (MSVC /fsanitize=address)" OFF)

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
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests/unit)
endif()
