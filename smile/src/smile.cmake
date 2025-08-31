set(SMILE_PATH ${CMAKE_CURRENT_LIST_DIR})
set(THIRDPARTY_PATH ${CMAKE_CURRENT_LIST_DIR}/../thirdparty)
set(CONAN_PATH ${CMAKE_CURRENT_LIST_DIR}/../../conan)

macro(add_module name)
    message("Adding module <" ${name} ">")
    add_subdirectory(${SMILE_PATH}/smile/${name} ${name})
endmacro()

macro(add_platform name)
    message("Adding platform <" ${name} ">")
    add_subdirectory(${SMILE_PATH}/platform/${name} ${name})
endmacro(add_platform)

function(smile_option name description default)

    if(DEFINED SMILE_${name}_DEFAULT)
        set(default ${SMILE_${name}_DEFAULT})
    endif()

    option(SMILE_${name} "${description}" ${default})
endfunction()

macro(get_files_recursively output_var)
    set(directories ${ARGV})
    list(REMOVE_AT directories 0)
    foreach( directory ${directories} )
        file(GLOB_RECURSE local_files CONFIGURE_DEPENDS "${directory}/**.cpp")

        list(APPEND ${output_var} ${local_files})
    endforeach()
endmacro()