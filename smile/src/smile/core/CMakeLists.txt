include(smile)

if(WIN32)
    add_platform(windows)
endif()

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/yaml-cpp/include
)

set(LIB_LINKS
    optimized ${THIRDPARTY_PATH}/yaml-cpp/x64/yaml-cpp.lib
    debug ${THIRDPARTY_PATH}/yaml-cpp/x64/yaml-cppd.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})