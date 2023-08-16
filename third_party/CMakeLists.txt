include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)
#set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# --------------------------------------------------------------------

if(TOOLBOX_TEST)
    FetchContent_Declare(googletest
            GIT_REPOSITORY	https://github.com/google/googletest.git
            GIT_TAG			main
            GIT_PROGRESS    true
            )

    FetchContent_MakeAvailable(googletest)
endif()
