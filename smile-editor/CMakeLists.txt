cmake_minimum_required(VERSION "3.25.0")

project(smile-editor)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(ENGINE_PATH "../smile-engine" ABSOLUTE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ENGINE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(smile-editor ${SOURCE_FILES})

include(${ENGINE_PATH}/smile-engine.cmake)

add_subdirectory(${ENGINE_PATH} engine)

add_module(common)
add_module(core)
add_module(graphic)
add_module(physics)
add_module(scripting)

add_platform(directx11)
add_platform(physx)

set(LIB_DIRS
    smile-engine
)

set(INCLUDE_DIRS
    src
    ${ENGINE_PATH}/thirdparty
)

target_include_directories(smile-editor PRIVATE ${INCLUDE_DIRS})
target_link_libraries(smile-editor PRIVATE ${LIB_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/mono"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mono"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

# Copy the dlls to the binary directory
add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

group_sources_by_folder(smile-editor)
set_property(TARGET smile-editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")