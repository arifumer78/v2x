if (V2X_BUILD_TESTS)
    include(FetchContent)

    set(WITH_DEMO OFF CACHE BOOL "" FORCE)
    set(ANTLR_BUILD_CPP_TESTS OFF CACHE BOOL "" FORCE)
    set(ANTLR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

    FetchContent_Declare(
        antlr4_cpp_runtime
        URL https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip
        PATCH_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/patch_antlr4_chrono.cmake
    )
    FetchContent_MakeAvailable(antlr4_cpp_runtime)

    add_executable(v2x_asn_parser_smoke_test
        tests/parser_smoke_test.cpp
        src/generated/AsnEtsiItsLexer.cpp
        src/generated/AsnEtsiItsParser.cpp
        src/generated/AsnEtsiItsParserBaseListener.cpp
        src/generated/AsnEtsiItsParserBaseVisitor.cpp
        src/generated/AsnEtsiItsParserListener.cpp
        src/generated/AsnEtsiItsParserVisitor.cpp
    )
    target_include_directories(v2x_asn_parser_smoke_test PRIVATE
        ${antlr4_cpp_runtime_SOURCE_DIR}/runtime/src
        src/generated
        grammar
    )
    target_link_libraries(v2x_asn_parser_smoke_test PRIVATE antlr4_static)
    target_compile_definitions(v2x_asn_parser_smoke_test PRIVATE ANTLR4CPP_STATIC)
    set_target_properties(v2x_asn_parser_smoke_test PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    if (MSVC)
        cmake_policy(SET CMP0091 NEW)
        set_target_properties(v2x_asn_parser_smoke_test PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
    endif()

    add_test(
        NAME AsnParserSmokeTest
        COMMAND v2x_asn_parser_smoke_test
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )
endif()
