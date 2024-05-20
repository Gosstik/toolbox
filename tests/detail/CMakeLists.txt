project_log("Build tests_detail." GREEN)

# Get sources.
get_filename_component(PATH_TO_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
file(GLOB_RECURSE SOURCES ${PATH_TO_SOURCES}/*.cpp)
file(GLOB_RECURSE HEADERS ${PATH_TO_SOURCES}/*.hpp)

# Make library.
add_library(tests_detail SHARED count_allocator.hpp)
set_target_properties(tests_detail PROPERTIES LINKER_LANGUAGE CXX)

project_log("Finished building tests_detail." MAGENTA)