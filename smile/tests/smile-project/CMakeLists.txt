cmake_minimum_required(VERSION "3.25.0")

project(smile-project)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(SMILE_PATH "../../src/smile" ABSOLUTE)
get_filename_component(CMAKE_PATH "../../cmake" ABSOLUTE)

include(${CMAKE_PATH}/prevent_in_source_builds.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SMILE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(smile-project ${SOURCE_FILES})

include(${SMILE_PATH}/smile.cmake)

add_subdirectory(${SMILE_PATH} engine)

add_module(common)
add_module(core)
add_module(graphic)
add_module(physics)
add_module(scripting)

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

target_include_directories(smile-project PRIVATE ${INCLUDE_DIRS})
target_link_libraries(smile-project PRIVATE ${LIB_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SMILE_PATH}/../../resources
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets
    )

# Copy the dlls to the binary directory
add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysX.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCommon.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXCooking.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CONAN_PATH}/$<CONFIG>/PhysXFoundation.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

include(${CMAKE_PATH}/group_sources_by_folder.cmake)
group_sources_by_folder(smile-project)

add_definitions(-DUNICODE)