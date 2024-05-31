# COLOR can be passed as additional argument.
function(toolbox_log MESSAGE)
    if(${ARGC} EQUAL 2)
        set(COLOR ${ARGV1})
        message_color_mode("[${PROJECT_NAME}] ${MESSAGE}" ${COLOR} STATUS)
    else()
        message(STATUS "[${PROJECT_NAME}] ${MESSAGE}")
    endif()
endfunction()

# for cmake debug
macro(toolbox_dev_log MESSAGE)
    toolbox_log("${MESSAGE}" BOLD_CYAN)
endmacro()


macro(toolbox_warn MESSAGE)
    message(WARNING "[${PROJECT_NAME}] ${MESSAGE}")
endmacro()

# --------------------------------------------------------------------
