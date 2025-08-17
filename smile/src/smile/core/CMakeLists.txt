include(smile)

if(WIN32)
    add_platform(windows)
endif()

find_package(yaml-cpp REQUIRED)

target_link_libraries(smile PUBLIC yaml-cpp)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})