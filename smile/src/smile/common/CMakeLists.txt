include(smile)

include(build/build_unit_tests.cmake)

find_package(fmt REQUIRED)

target_link_libraries(smile PUBLIC fmt::fmt)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})