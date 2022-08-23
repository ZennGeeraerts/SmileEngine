cmake_minimum_required(VERSION 3.14)

project(smile-engine)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT smile-editor)
endif()

# No in source build allowed
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source build are not allowed. Make a new directory called build and run CMake from there.")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -g3 -O0 -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX") #Using Visual Studio C++
endif()

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
      file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                    "${CMAKE_BINARY_DIR}/conan.cmake"
                    EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                    TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_add_remote(NAME conan-center
                 INDEX 0
                 URL https://center.conan.io
                 VERIFY_SSL True)

option(WITH_SMILERASTER "With SmileRaster" ON)
if(WITH_SMILERASTER)
    add_compile_definitions( WITH_SMILERASTER )
    add_subdirectory(smile-raster)
endif()

add_subdirectory(smile-engine)
add_subdirectory(thirdparty/dx11effects)
add_subdirectory(thirdparty/dx11tex)

add_subdirectory(smile-editor)
#add_subdirectory(smile-project)