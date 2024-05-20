begin_build_test()

add_test_playground(play)

set(TEST_EXECUTABLES "")
add_test_suite(unit_tests TEST_EXECUTABLES unit.cpp)
add_test_suite(stress_tests TEST_EXECUTABLES stress.cpp)

# Uncomment next line to make coverage of tested header.
#make_header_coverage(${TEST_EXECUTABLES})

end_build_test()

# --------------------------------------------------------------------
