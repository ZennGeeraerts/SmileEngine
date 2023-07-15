cmake_minimum_required(VERSION "3.20.0")

project(smile-engine)
set(CMAKE_CXX_STANDARD 17)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT smile-editor)

# Prevents in source build
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -g3 -O0 -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX") #Using Visual Studio C++
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(smile-engine/thirdparty/imgui)

add_subdirectory(smile-raster)
add_subdirectory(smile-script-core)
add_subdirectory(smile-engine)
add_subdirectory(smile-editor)
add_subdirectory(smile-project)
add_subdirectory(unit-tests)