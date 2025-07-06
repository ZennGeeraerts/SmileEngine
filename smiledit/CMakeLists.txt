cmake_minimum_required(VERSION "3.25.0")

project(smiledit)

get_filename_component(SMILE_PATH "../smile/src" ABSOLUTE)
get_filename_component(CMAKE_PATH "../cmake" ABSOLUTE)

include(${CMAKE_PATH}/prevent_in_source_builds.cmake)
include(${CMAKE_PATH}/compiler_flags.cmake)

set_compiler_flags()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SMILE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(smiledit ${SOURCE_FILES})

include(${SMILE_PATH}/smile.cmake)

add_subdirectory(${SMILE_PATH} smile)

add_module(common)
add_module(core)
add_module(graphic)
add_module(physics)
add_module(scripting)
add_module(blueprint)

add_platform(directx11)
add_platform(physx)

set(LIB_DIRS
    smile
)

set(INCLUDE_DIRS
    src
    ${SMILE_PATH}
    ${THIRDPARTY_PATH}
)

target_include_directories(smiledit PRIVATE ${INCLUDE_DIRS})
target_link_libraries(smiledit PRIVATE ${LIB_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SMILE_PATH}/../resources"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/mono"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mono"
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

# Copy the dlls to the binary directory
add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysX.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCommon.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCooking.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smiledit
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXFoundation.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

include(${CMAKE_PATH}/group_sources_by_folder.cmake)
group_sources_by_folder(smiledit)

set_property(TARGET smiledit PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")