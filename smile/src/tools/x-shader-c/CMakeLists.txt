cmake_minimum_required(VERSION "3.25.0")

get_filename_component(SMILE_PATH "../" ABSOLUTE)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(x-shader-c ${SOURCE_FILES})

set(INCLUDE_DIRS
    thirdparty/dxc/include
    ${SMILE_PATH}
    ${THIRDPARTY_PATH}
)

set(LIB_DIRS
    smile
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/lib/x64/dxcompiler.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/lib/x64/dxil.lib
    d3dcompiler
)

target_include_directories(x-shader-c PRIVATE ${INCLUDE_DIRS})
target_link_libraries(x-shader-c PRIVATE ${LIB_DIRS})

add_custom_command(
    TARGET x-shader-c
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/bin/x64/dxcompiler.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET x-shader-c
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/bin/x64/dxil.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )