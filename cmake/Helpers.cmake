# Prepend ${PREFIX} to all arguments and put them in ${VAR}
# Usage: prepend(FILES "${DIR}/" ${NAMES})

function(prepend VAR PREFIX)
    set(LIST_VAR "")
    foreach(ELEM ${ARGN})
        list(APPEND LIST_VAR "${PREFIX}${ELEM}")
    endforeach()
    set(${VAR} "${LIST_VAR}" PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

macro(toolbox_get_play_target VAR)
  set(${VAR} ${TEST_PREFIX}_play)
    # if (NOT DEFINED USE_CMAKE_DIR_AS_SECTION)
    #   set(${VAR} ${SECTION_NAME}_${TEST_NAME}_play)
    # else()
    #   set(${VAR} ${SECTION_NAME}_play)
    # endif()
endmacro()

# --------------------------------------------------------------------

macro(toolbox_get_test_target VAR NAME)
    set(${VAR} ${TEST_PREFIX}_${NAME})
endmacro()

# --------------------------------------------------------------------

macro(get_header_coverage_target VAR)
    set(${VAR} "lcov_${TEST_PREFIX}_hpp")
endmacro()

# --------------------------------------------------------------------

function(extract_name_after_tests OUT PATH TAG)
    string(REPLACE "/" ";" RES "${PATH}")
    list(REVERSE RES)

    # Search for the start of actual target in list.
    list(FIND RES "tests" IND)
    if(${IND} EQUAL -1)
        message(FATAL_ERROR "Unable to extract_name_after_tests.\
                             PATH = ${PATH}, RES = ${RES}")
    endif()
    if (${IND} EQUAL "0")
        if (${TAG} STREQUAL "COVERAGE")
            # this means that we are in `tests` dir. May occur only
            # with get_directory_coverage_target.
            set(${OUT} "toolbox" PARENT_SCOPE)
            return()
        endif()
        # Else it is UB.
        message(FATAL_ERROR "Unable to extract_name_after_tests.\
                             PATH = ${PATH}, IND = 1, TAG = ${TAG}")
    endif()

    # Extract target name from list.
    list(SUBLIST RES "0" ${IND} RES)
    list(REVERSE RES)
    string(REPLACE ";" "_" RES "${RES}")

    # Save result to OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

function(get_directory_coverage_target OUT CMAKE_CURRENT_SOURCE_DIR_IN)
    # Basic work.
    extract_name_after_tests(RES ${CMAKE_CURRENT_SOURCE_DIR_IN} COVERAGE)

    # Add prefix and suffix.
    set(RES "lcov_${RES}_dir")

    # Save result in OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

# For tests' targets.

function(get_section_name OUT SECTION_DIR)
    # Basic work.
    extract_name_after_tests(RES ${SECTION_DIR} SECTION)

    # Save result in OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

function(replace_tests_with_toolbox OUT PATH)
    # Make list from path and reverse it.
    string(REPLACE "/" ";" RES "${PATH}")
    list(REVERSE RES)

    # Find `tests` to `toolbox`.
    list(FIND RES "tests" IND)
    if(${IND} EQUAL -1)
        set(MSG_STR "Unable to replace_tests_in_path.")
        set(MSG_STR "${MSG_STR} PATH = ${PATH}, RES = \"${RES}\".")
        message(FATAL_ERROR ${MSG_STR})
    endif()

    # Replace `tests` with `toolbox` and reverse list back.
    list(REMOVE_AT RES ${IND})
    list(INSERT RES ${IND} "toolbox")
    list(REVERSE RES)

    # Make path from list.
    string(REPLACE ";" "/" RES "${RES}")

    # Save result to OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

function(get_header_for_coverage OUT HEADER_DIR TEST_NAME)
    # Add header to path.
    string(REPLACE "/" ";" RES "${HEADER_DIR}")
    list(APPEND RES "${TEST_NAME}.hpp")
    string(REPLACE ";" "/" RES "${RES}")

    # Save result in OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------

function(get_directory_for_coverage OUT CMAKE_CURRENT_SOURCE_DIR_IN)
    # Basic work.
    replace_tests_with_toolbox(RES "${CMAKE_CURRENT_SOURCE_DIR_IN}")

    # Save result in OUT.
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()

# --------------------------------------------------------------------
