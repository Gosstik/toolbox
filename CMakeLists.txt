cmake_minimum_required(VERSION 3.22)
project(toolbox)

# --------------------------------------------------------------------

# Options.

option(TOOLBOX_TEST "Enable toolbox tests." OFF)
option(TOOLBOX_DEVELOPMENT "Toolbox development mode." OFF)
option(TOOLBOX_COVERAGE "Toolbox enable coverage." OFF)

# Supress warnings from third-party/CodeCoverage
if (TOOLBOX_COVERAGE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# --------------------------------------------------------------------

# Set environment.

set(TEST_DEPENDENCIES toolbox GTest::gtest_main)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(Color)
include(Logging)
include(Helpers)
include(BuildTest)
include(third-party/CodeCoverage)
include(Coverage)

add_subdirectory(third_party)

# This must be included after library so that third party code is not affected.
include(CompileOptions)

# Uncomment next lines to save binary files in special dirs.
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# --------------------------------------------------------------------

# Build library.

add_subdirectory(toolbox)

# --------------------------------------------------------------------

# Enable playground and tests.

if(TOOLBOX_DEVELOPMENT)
    add_subdirectory(playground)
endif()

if(TOOLBOX_TEST OR TOOLBOX_DEVELOPMENT OR TOOLBOX_COVERAGE)
    add_subdirectory(tests)
endif()

# --------------------------------------------------------------------
