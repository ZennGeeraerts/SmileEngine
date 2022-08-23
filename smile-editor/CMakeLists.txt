cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(smile-editor ${SOURCE_FILES})

set_target_properties(smile-engine PROPERTIES OUTPUT_NAME smile-editor)

target_link_libraries(smile-editor PRIVATE smile-engine)

target_include_directories(smile-editor 
	PUBLIC ${CMAKE_SOURCE_DIR}/smile-engine/src
	)