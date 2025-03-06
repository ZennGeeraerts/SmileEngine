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