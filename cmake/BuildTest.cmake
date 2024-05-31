# Prologue

# Sets TEST_DIR, TEST_NAME, SECTION_DIR, SECTION_NAME.
#
# Description:
# Let `target_header` be the file (header) we want to test, then:
#
# TEST_DIR     == path to directory where tests for `target_header` located.
# TEST_NAME    == name of `target_header`.
# SECTION_DIR  == path to directory above TEST_DIR
# SECTION_NAME == name of folders where `target_header` located relative
#                 to `toolbox`, written in snake_case.
macro(begin_build_test)
    # Test variables
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    toolbox_log("Start processing ${TEST_DIR}." GREEN)
    get_filename_component(TEST_NAME ${TEST_DIR} NAME)

    # Section variables
    get_filename_component(SECTION_DIR ${TEST_DIR} DIRECTORY)
    get_section_name(SECTION_NAME ${SECTION_DIR})
endmacro()

# --------------------------------------------------------------------

# Playground

function(add_test_playground PLAY_DIR)
    if(TOOLBOX_DEVELOPMENT AND EXISTS "${TEST_DIR}/${PLAY_DIR}")
        toolbox_get_play_target(PLAY_TARGET_NAME)
        toolbox_log("Add test playground: ${PLAY_TARGET_NAME}")

        # Get playground sources
        file(GLOB_RECURSE PLAY_SOURCES
                ${CMAKE_CURRENT_SOURCE_DIR}/${PLAY_DIR}/*.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/${PLAY_DIR}/*.cpp)
        toolbox_log("PLAY_SOURCES = ${PLAY_SOURCES}")

        # Make executable
        add_executable(${PLAY_TARGET_NAME} ${PLAY_SOURCES})
        target_link_libraries(${PLAY_TARGET_NAME} PRIVATE toolbox)
        toolbox_log("Test playground was added successfully.")
    endif()
endfunction()

# --------------------------------------------------------------------

# Test suite

function(add_test_suite SUITE_NAME TEST_EXECUTABLES_OUT)
    if(TOOLBOX_TEST)
        toolbox_get_test_target(TEST_TARGET_NAME ${SUITE_NAME})
        toolbox_log("Add test suite: ${TEST_TARGET_NAME}")

        # Get test sources
        prepend(TEST_SOURCES "${TEST_DIR}/" ${ARGN})

        # Make test target
        toolbox_log("TEST_SOURCES = ${TEST_SOURCES}")
        add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
        target_link_libraries(${TEST_TARGET_NAME} PUBLIC ${TEST_DEPENDENCIES})
        gtest_discover_tests(${TEST_TARGET_NAME})

        # Append test target to TEST_EXECUTABLES_OUT
        list(APPEND ${TEST_EXECUTABLES_OUT} ${TEST_TARGET_NAME})
        set(${TEST_EXECUTABLES_OUT} ${${TEST_EXECUTABLES_OUT}} PARENT_SCOPE)
        toolbox_log("Test suite was added successfully.")
    endif()
endfunction()

# --------------------------------------------------------------------

# Epilogue

macro(end_build_test)
    # Test variables
    toolbox_log("Finished processing ${TEST_DIR}." MAGENTA)
endmacro()
