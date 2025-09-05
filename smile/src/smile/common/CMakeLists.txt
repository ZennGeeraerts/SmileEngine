include(smile)

include(build/build_unit_tests.cmake)

if(WIN32)
    add_platform(windows)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

find_package(fmt REQUIRED)

target_link_libraries(smile PUBLIC fmt::fmt)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})