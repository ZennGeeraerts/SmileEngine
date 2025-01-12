cmake_minimum_required(VERSION "3.25.0")

project(unit-tests)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(SMILE_PATH "../../src/smile" ABSOLUTE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SMILE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES main.cpp)

add_executable(smile ${SOURCE_FILES})
set_target_properties(smile PROPERTIES OUTPUT_NAME unit-tests)

include(${SMILE_PATH}/smile.cmake)

add_subdirectory(${SMILE_PATH} engine)

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
        ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysX.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCommon.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCooking.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXFoundation.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

group_sources_by_folder(smile)
add_definitions(-DUNICODE)