cmake_minimum_required(VERSION "3.25.0")

project(sprite-test)

set(SMILE_SERIALIZER OFF)
set(SMILE_ANIMATION OFF)
set(SMILE_IMGUI OFF)
set(SMILE_SCENE OFF)

get_filename_component(SMILE_PATH "../../src" ABSOLUTE)
get_filename_component(CMAKE_PATH "../../../cmake" ABSOLUTE)

include(${CMAKE_PATH}/prevent_in_source_builds.cmake)
include(${CMAKE_PATH}/compiler_flags.cmake)

set_compiler_flags()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SMILE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

set(APPLICATION_TARGET sprite-test)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(sprite-test ${SOURCE_FILES})

include(${SMILE_PATH}/smile.cmake)

add_subdirectory(${SMILE_PATH} smile)

add_module(common)
add_module(core)
add_module(graphic)

add_platform(d3dcommon)
add_platform(d3d11)

set(LIB_DIRS
    smile
)

set(INCLUDE_DIRS
    src
    ${SMILE_PATH}
    ${THIRDPARTY_PATH}
)

target_include_directories(sprite-test PRIVATE ${INCLUDE_DIRS})
target_link_libraries(sprite-test PRIVATE ${LIB_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET sprite-test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SMILE_PATH}/../resources"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
    )

include(${CMAKE_PATH}/group_sources_by_folder.cmake)
group_sources_by_folder(sprite-test)

add_definitions(-DUNICODE)