project_log("Build toolbox library" GREEN)
get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

# Get sources
project_log("Get sources")
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

# Build library
project_log("Build library")
add_library(toolbox STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(toolbox PUBLIC ${LIB_INCLUDE_PATH})
set_target_properties(toolbox PROPERTIES LINKER_LANGUAGE CXX)

project_log("Finished making toolbox library" MAGENTA)
