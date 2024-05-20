# Make COVERAGE_EXCLUDE.

set(EXCLUDE_PREFIX "${PROJECT_SOURCE_DIR}/")
prepend(COVERAGE_EXCLUDES ${EXCLUDE_PREFIX}
        "tests*"
        "playground*"
        "build*"
        "cmake-build*"
        "*/googletest/*"
        "*/googlemock/*")
list(APPEND COVERAGE_EXCLUDES "/usr*")

# --------------------------------------------------------------------

# Make COVERAGE_LCOV_ARGS.

list(APPEND COVERAGE_LCOV_ARGS "--no-external")

# --------------------------------------------------------------------

# Header coverage.

function(make_header_coverage TEST_EXECUTABLES)
    if(TOOLBOX_COVERAGE)
        replace_tests_with_toolbox(HEADER_DIR "${SECTION_DIR}")
        get_header_for_coverage(HEADER "${HEADER_DIR}" "${TEST_NAME}")
        get_header_coverage_target(COVERAGE_TARGET)
        project_log("Make coverage target: ${COVERAGE_TARGET}." CYAN)

        get_filename_component(HEADER_DIR "${HEADER}" DIRECTORY)
        file(GLOB_RECURSE EXCLUDE_FILES
                "${HEADER_DIR}/*.hpp"
                "${HEADER_DIR}/*.cpp")

        list(REMOVE_ITEM EXCLUDE_FILES "${HEADER}")

        setup_target_for_coverage_lcov(
                NAME "${COVERAGE_TARGET}"
                EXECUTABLE ${TEST_EXECUTABLES}
                BASE_DIRECTORY "${HEADER_DIR}"
                EXCLUDE ${EXCLUDE_FILES} ${COVERAGE_EXCLUDES}
                LCOV_ARGS ${COVERAGE_LCOV_ARGS}
        )
        project_log("Coverage target was made successfully." CYAN)
    endif()
endfunction()

# --------------------------------------------------------------------

# Directory coverage.

# NOTE: It's too burdensome to extract tests' targets so we create
# new target EXE_COVERAGE_TARGET specially for coverage.
function(make_directory_coverage CMAKE_CURRENT_SOURCE_DIR_IN)
    if(TOOLBOX_COVERAGE)
        get_directory_for_coverage(COVERAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR_IN})
        get_directory_coverage_target(COVERAGE_TARGET ${CMAKE_CURRENT_SOURCE_DIR_IN})
        set(EXE_COVERAGE_TARGET "exe_${COVERAGE_TARGET}")

        project_log("Make directory coverage: ${COVERAGE_TARGET}." CYAN)

        # Find coverage sources.
        file(GLOB_RECURSE COVERAGE_SOURCE
                "${COVERAGE_SOURCE_DIR}/*.hpp"
                "${COVERAGE_SOURCE_DIR}/*.cpp")

        # Find test sources.
        set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR_IN}")
        file(GLOB_RECURSE TEST_SOURCES
                "${TEST_SOURCE_DIR}/*.hpp"
                "${TEST_SOURCE_DIR}/*.cpp")

        # Find sources of playgrounds.
        string(LENGTH "${TEST_SOURCE_DIR}/" PREFIX_LEN)
        set(CUR_IND "0")
        foreach(PATH ${TEST_SOURCES})
            string(SUBSTRING ${PATH} ${PREFIX_LEN} "-1" RELATIVE_PATH)
            string(FIND "${RELATIVE_PATH}" "play" IND)
            if (NOT ${IND} EQUAL "-1")
                list(APPEND USELESS_INDS "${CUR_IND}")
            endif()
            math(EXPR CUR_IND "${CUR_IND} + 1")
        endforeach()

        # Remove sources of playgrounds from test sources.
        list(REMOVE_AT TEST_SOURCES ${USELESS_INDS})


#        set(TEST_SOURCES "/home/ownstreamer/proga/toolbox/tests/std/concepts/unit.cpp")

        # Make executable from test sources.
        add_executable(${EXE_COVERAGE_TARGET} ${TEST_SOURCES})
        target_link_libraries(${EXE_COVERAGE_TARGET} PUBLIC ${TEST_DEPENDENCIES})
        gtest_discover_tests(${EXE_COVERAGE_TARGET})

        # Make lcov target.
        setup_target_for_coverage_lcov(
                NAME "${COVERAGE_TARGET}"
                EXECUTABLE "${EXE_COVERAGE_TARGET}"
#                BASE_DIRECTORY "${COVERAGE_SOURCE_DIR}"
                BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
                EXCLUDE ${COVERAGE_EXCLUDES}
                LCOV_ARGS ${COVERAGE_LCOV_ARGS}
        )
        project_log("Directory coverage was made successfully." CYAN)
    endif()
endfunction()

# --------------------------------------------------------------------

# Commands for commandline.

#lcov --directory . --capture --output-file coverage.info
#genhtml --demangle-cpp -o coverage coverage.info

#lcov --directory ./build --capture --output-file ./coverage.info -rc lcov_branch_coverage=1
#genhtml coverage.info --branch-coverage --output-directory ./code_coverage_report/
#/ -d --directory
#/ -c --capture
#/ --rc lcov_branch_coverage=1

# --------------------------------------------------------------------

# Guides from the Internet.

#if(ENABLE_COVERAGE)
#    # set compiler flags
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -coverage")
#
#    # find required tools
#    find_program(LCOV lcov REQUIRED)
#    find_program(GENHTML genhtml REQUIRED)
#
#    # add coverage target
#    add_custom_target(coverage
#            # gather data
#            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
#            # generate report
#            COMMAND ${GENHTML} --demangle-cpp -o coverage coverage.info
#            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
#endif()

# --------------------------------------------------------------------

## ----------------------------------------------------
#
#cmake_minimum_required(VERSION 3.5)
#project(CMake_GCov CXX)
#
## Set the compiler options
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
#
## Create OBJECT_DIR variable
#set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/RunAdder.dir)
#message("-- Object files will be output to: ${OBJECT_DIR}")
#
## Set the sources
#set(SOURCES
#        RunAdder.cpp
#        Adder.cpp
#)
#
## Create the executable
#add_executable(RunAdder ${SOURCES})
#
## Create the gcov target. Run coverage tests with 'make gcov'
#add_custom_target(gcov
#        COMMAND mkdir -p coverage
#        COMMAND ${CMAKE_MAKE_PROGRAM} test
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)
#add_custom_command(TARGET gcov
#        COMMAND echo "=================== GCOV ===================="
#        COMMAND gcov -b ${CMAKE_SOURCE_DIR}/*.cpp -o ${OBJECT_DIR}
#        | grep -A 5 "Adder.cpp" > CoverageSummary.tmp
#        COMMAND cat CoverageSummary.tmp
#        COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/coverage"
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
#)
#add_dependencies(gcov RunAdder)
## Make sure to clean up the coverage folder
#set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES coverage)
#
## Create the gcov-clean target. This cleans the build as well as generated
## .gcda and .gcno files.
#add_custom_target(scrub
#        COMMAND ${CMAKE_MAKE_PROGRAM} clean
#        COMMAND rm -f ${OBJECT_DIR}/*.gcno
#        COMMAND rm -f ${OBJECT_DIR}/*.gcda
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)
#
## Testing
#enable_testing()
#
#add_test(output_test ${CMAKE_CURRENT_BINARY_DIR}/RunAdder)
#set_tests_properties(output_test PROPERTIES PASS_REGULAR_EXPRESSION "0;5;10")