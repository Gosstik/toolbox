include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Create file FullyDisconnected.cmake and paste code
# to enable/disable offline mode:

# set(FETCHCONTENT_FULLY_DISCONNECTED OFF) # default: OFF

set(FULLY_DISCONNECTED_FILE "${CMAKE_CURRENT_SOURCE_DIR}/FullyDisconnected.cmake")

if(EXISTS "${FULLY_DISCONNECTED_FILE}")
  toolbox_log("Found FullyDisconnected.cmake" GREEN)
  include("${FULLY_DISCONNECTED_FILE}")
else()
  toolbox_warn("FullyDisconnected.cmake is not found, using default")
  set(FETCHCONTENT_FULLY_DISCONNECTED OFF)
endif()

# --------------------------------------------------------------------

if(TOOLBOX_TEST)
  FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    GIT_PROGRESS true
  )

  FetchContent_MakeAvailable(googletest)
endif()
