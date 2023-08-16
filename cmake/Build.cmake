# Prologue

macro(begin_build_test)
    # Test variables
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    project_log("Start processing ${TEST_DIR}" GREEN)
    get_filename_component(TEST_NAME ${TEST_DIR} NAME)

    # Section variables
    get_filename_component(SECTION_DIR ${TEST_DIR} DIRECTORY)
    get_section_name(SECTION_NAME ${SECTION_DIR})

    set(TEST_LIST "")
endmacro()

# --------------------------------------------------------------------

# Playground

function(add_test_playground PLAY_DIR)
    if(TOOLBOX_DEVELOPMENT AND EXISTS "${TEST_DIR}/${PLAY_DIR}")
        get_play_target(PLAY_TARGET_NAME)
        project_log("Add test playground: ${PLAY_TARGET_NAME}")

        # Get playground sources
        file(GLOB_RECURSE PLAY_SOURCES
                ${CMAKE_CURRENT_SOURCE_DIR}/${PLAY_DIR}/*.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/${PLAY_DIR}/*.cpp)
        project_log("PLAY_SOURCES = ${PLAY_SOURCES}")

        # Make executable
        add_executable(${PLAY_TARGET_NAME} ${PLAY_SOURCES})
        target_link_libraries(${PLAY_TARGET_NAME} PRIVATE toolbox)
    endif()
endfunction()

# --------------------------------------------------------------------

# Test suite
# Additional parameter "NO_TEST_NAME" means that
function(add_test_suite SUITE_NAME)
    if(TOOLBOX_TEST)
        get_test_target(TEST_TARGET_NAME ${SUITE_NAME})
        project_log("Add test suite: ${TEST_TARGET_NAME}")

        # Get test sources
        prepend(TEST_SOURCES "${TEST_DIR}/" ${ARGN})

        # Make test target
        project_log("TEST_SOURCES = ${TEST_SOURCES}")
        add_executable(${TEST_TARGET_NAME} ${TEST_SOURCES})
        target_link_libraries(${TEST_TARGET_NAME} PRIVATE toolbox GTest::gtest_main)
        gtest_discover_tests(${TEST_TARGET_NAME})
    endif()
endfunction()

# --------------------------------------------------------------------

# Epilogue

macro(end_build_test)
    # Test variables
    project_log("Finished processing ${TEST_DIR}" MAGENTA)
endmacro()
