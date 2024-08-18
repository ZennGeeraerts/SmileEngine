cmake_minimum_required(VERSION "3.25.0")

project(unit-tests)
set(CMAKE_CXX_STANDARD 17)

get_filename_component(ENGINE_PATH "../../" ABSOLUTE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ENGINE_PATH})
message(STATUS "Engine main path: " ${CMAKE_MODULE_PATH})

file(GLOB_RECURSE SOURCE_FILES *.cpp thirdparty/catch.hpp)

add_executable(unit-tests ${SOURCE_FILES})

include(${ENGINE_PATH}/smile-engine.cmake)

add_subdirectory(${ENGINE_PATH} engine)

add_module(common)
add_module(core)

set(LIB_DIRS
    smile-engine
)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/thirdparty
)

target_link_libraries(unit-tests PRIVATE ${LIB_DIRS})
target_include_directories(unit-tests PRIVATE ${INCLUDE_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

group_sources_by_folder(unit-tests)
set_property(TARGET unit-tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(unit-tests PROPERTIES FOLDER Tools)