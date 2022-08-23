cmake_minimum_required(VERSION 3.14)

project(smile-raster LANGUAGES CXX CUDA)
enable_language(CUDA)

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/*.cu
)

find_package(CUDAToolkit REQUIRED)

add_library(smile-raster STATIC ${SOURCE_FILES})
target_include_directories(smile-raster PUBLIC ${CMAKE_SOURCE_DIR}/smile-raster/src)