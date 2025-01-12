cmake_minimum_required(VERSION "3.25.0")

project(unit-tests)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(ENGINE_PATH "../../src/smile" ABSOLUTE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ENGINE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES main.cpp)

add_executable(smile ${SOURCE_FILES})
set_target_properties(smile PROPERTIES OUTPUT_NAME unit-tests)

include(${ENGINE_PATH}/smile.cmake)

add_subdirectory(${ENGINE_PATH} engine)

add_module(common)
add_module(core)
add_module(graphic)
add_module(physics)
add_module(scripting)

add_platform(directx11)
add_platform(physx)

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${ENGINE_PATH}/../../thirdparty/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${ENGINE_PATH}/../../conan/$<CONFIG>/PhysX.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${ENGINE_PATH}/../../conan/$<CONFIG>/PhysXCommon.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${ENGINE_PATH}/../../conan/$<CONFIG>/PhysXCooking.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${ENGINE_PATH}/../../conan/$<CONFIG>/PhysXFoundation.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

group_sources_by_folder(smile)
add_definitions(-DUNICODE)