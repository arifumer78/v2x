cmake_minimum_required(VERSION 3.28)
project(v2x LANGUAGES CXX)

option(V2X_BUILD_TESTS "Build unit tests" ON)

if (V2X_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(runtime)
add_subdirectory(compiler)
