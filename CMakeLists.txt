cmake_minimum_required(VERSION 3.22)
project(toolbox)

# --------------------------------------------------------------------

# Options.
option(TOOLBOX_TEST "Enable toolbox tests." OFF)
option(TOOLBOX_DEVELOPMENT "Toolbox development mode." OFF)
option(TOOLBOX_COVERAGE "Toolbox enable coverage." OFF)

if(TOOLBOX_COVERAGE)
  # Supress warnings from third-party/CodeCoverage
  set(CMAKE_BUILD_TYPE "Debug")

  if(NOT TOOLBOX_TEST)
    message(WARNING "[toolbox] !!! Enabling TOOLBOX_TEST because TOOLBOX_COVERAGE=ON")
    set(TOOLBOX_TEST ON)
  endif()
endif()

# if(TOOLBOX_DEVELOPMENT)
#   if(NOT TOOLBOX_TEST)
#     message(WARNING "[toolbox] !!! Enabling TOOLBOX_TEST because TOOLBOX_DEVELOPMENT=ON")
#     set(TOOLBOX_TEST ON)
#   endif()
# endif()

# --------------------------------------------------------------------

# Set environment.
set(TEST_DEPENDENCIES toolbox GTest::gtest_main)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(Color)
include(Logging)
include(Helpers)
include(BuildTest)
include(Coverage)

add_subdirectory(third_party)

# This must be included after library so that third party code is not affected.
include(CompileOptions)

# Uncomment next lines to save binary files in special dirs.
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# --------------------------------------------------------------------

# Build library.
add_subdirectory(toolbox)

# --------------------------------------------------------------------

# Enable playground and tests.
if(TOOLBOX_DEVELOPMENT AND EXISTS playground)
  add_subdirectory(playground)
endif()

if(TOOLBOX_TEST OR TOOLBOX_DEVELOPMENT OR TOOLBOX_COVERAGE)
  add_subdirectory(tests)
endif()

# add_subdirectory(tests)

# --------------------------------------------------------------------
