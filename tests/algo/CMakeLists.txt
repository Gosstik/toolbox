# add_subdirectory(subset)

# --------------------------------------------------------------------

set(USE_CMAKE_DIR_AS_SECTION "true")
begin_build_test()

# add_test_playground(play)

set(TEST_EXECUTABLES "")
add_test_suite(subset_tests TEST_EXECUTABLES subset.cpp)

# Uncomment next line to make coverage of tested header.
#make_header_coverage(${TEST_EXECUTABLES})

end_build_test()

# --------------------------------------------------------------------

