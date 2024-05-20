enable_testing()

include(GoogleTest)

#add_subdirectory(detail)
add_subdirectory(algo)
add_subdirectory(container)
add_subdirectory(intrusive)
add_subdirectory(std)

# Uncomment next line to make coverage of all library.
make_directory_coverage(${CMAKE_CURRENT_SOURCE_DIR})

# --------------------------------------------------------------------
