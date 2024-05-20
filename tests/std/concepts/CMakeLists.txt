begin_build_test()

add_test_playground(play)

set(TEST_EXECUTABLES "")
add_test_suite(unit_tests TEST_EXECUTABLES unit.cpp)

# NOTE: coverage is not working for metaprogramming.
# Uncomment next line to make coverage of tested header.
#make_header_coverage(${TEST_EXECUTABLES})

end_build_test()

# --------------------------------------------------------------------
