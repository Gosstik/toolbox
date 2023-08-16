cmake_minimum_required(VERSION 3.25)
project(toolbox)

# --------------------------------------------------------------------

# Set environment

include(cmake/Color.cmake)
include(cmake/Logging.cmake)
include(cmake/Helpers.cmake)
include(cmake/Build.cmake)

option(TOOLBOX_TEST "Enable toolbox tests" OFF)
option(TOOLBOX_DEVELOPMENT "Toolbox development mode" OFF)

add_subdirectory(third_party)

# This must be included after library so that
# third party code is not affected
include(cmake/CompileOptions.cmake)

# Uncomment next lines to save binary files in special folders
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# --------------------------------------------------------------------

# Make library

add_subdirectory(toolbox)

# --------------------------------------------------------------------

# Enable playground and tests

if(TOOLBOX_DEVELOPMENT)
    add_subdirectory(playground)
endif()

if(TOOLBOX_TEST OR TOOLBOX_DEVELOPMENT)
    add_subdirectory(tests)
endif()

# --------------------------------------------------------------------
