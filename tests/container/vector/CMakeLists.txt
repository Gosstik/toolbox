begin_build_test()

add_test_playground(play)

add_test_suite(unit_tests unit.cpp)
add_test_suite(stress_tests stress.cpp)

end_build_test()
