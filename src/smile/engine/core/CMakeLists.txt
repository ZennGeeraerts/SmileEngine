include(smile-engine)

if(WIN32)
    add_platform(windows)
endif()

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
    ${ENGINE_PATH}/../../thirdparty/yaml-cpp/include
)

set(LIB_LINKS
    optimized ${ENGINE_PATH}/../../thirdparty/yaml-cpp/x64/yaml-cpp.lib
    debug ${ENGINE_PATH}/../../thirdparty/yaml-cpp/x64/yaml-cppd.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})