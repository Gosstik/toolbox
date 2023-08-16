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

macro(get_play_target VAR)
    set(${VAR} ${SECTION_NAME}_${TEST_NAME}_play)
endmacro()

# --------------------------------------------------------------------

macro(get_test_target VAR NAME)
    set(${VAR} ${SECTION_NAME}_${TEST_NAME}_${NAME})
endmacro()

# --------------------------------------------------------------------

# For tests' targets

function(get_section_name OUT TEST_DIR)
    # Make list of folders in TEST_DIR
    set(RES "")
    string(REPLACE "/" ";" RES "${TEST_DIR}")

    # Search for the start of SECTION_NAME in list
    set(IND -1)
    list(FIND RES "tests" IND)
    if(${IND} EQUAL -1)
        message(FATAL_ERROR "Unable to get section name. RES = ${RES}, TEST_DIR = ${TEST_DIR}")
    endif()
    math(EXPR IND "${IND} + 1")

    # Extract SECTION_NAME from list
    set(LEN -1)
    list(LENGTH RES LEN)
    math(EXPR LEN "${LEN} - ${IND}")
    list(SUBLIST RES ${IND} ${LEN} RES)
    string(REPLACE ";" "_" RES "${RES}")

    # Save result in OUT
    set(${OUT} ${RES} PARENT_SCOPE)
endfunction()