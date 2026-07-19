add_executable(v2x_per_tests
    test_smoke.cpp
    test_result.cpp
    test_bit_cursor.cpp
    test_bit_primitives.cpp
    test_constrained_whole_number.cpp
    test_semi_constrained_whole_number.cpp
    test_unconstrained_whole_number.cpp
    test_length_determinant.cpp
    test_normally_small_length.cpp
    test_open_type.cpp
    test_policy_wiring.cpp
    test_octet_string.cpp
    test_bit_string.cpp
    test_sequence_of.cpp
    test_open_type_wrap.cpp
    test_sequence_extension.cpp
    test_enumerated.cpp
    test_choice.cpp
    test_worked_example_sequence.cpp
    test_x691_spec_examples.cpp
    test_extensible_constraints.cpp
)
target_link_libraries(v2x_per_tests PRIVATE v2x_per GTest::gtest_main)
set_target_properties(v2x_per_tests PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
gtest_discover_tests(v2x_per_tests)
