cmake_minimum_required(VERSION "3.22")

project("SmileEditor")

get_filename_component(ENGINE_PATH "../SmileEngine" ABSOLUTE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ENGINE_PATH})

add_executable("${PROJECT_NAME}" "src/SmileEditor.cpp")