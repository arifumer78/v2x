add_executable(v2x_per_property_tests
    test_property_integers.cpp
    test_property_length_and_bits.cpp
    test_property_strings.cpp
    test_property_sequence_of.cpp
    test_property_enumerated_choice.cpp
    test_double_round_trip.cpp
)
target_link_libraries(v2x_per_property_tests PRIVATE v2x_per GTest::gtest_main)
set_target_properties(v2x_per_property_tests PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
gtest_discover_tests(v2x_per_property_tests)
