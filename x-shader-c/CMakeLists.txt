cmake_minimum_required(VERSION "3.25.0")

project(x-shader-c)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(x-shader-c ${SOURCE_FILES})

find_package(yaml-cpp REQUIRED)

set(INCLUDE_DIRS
    thirdparty/dxc/include
)

set(LIB_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/lib/x64/dxcompiler.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/dxc/lib/x64/dxil.lib
    yaml-cpp
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