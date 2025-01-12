include(smile)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/../../thirdparty/mono/include
)

set(LIB_LINKS
    ${ENGINE_PATH}/../../thirdparty/mono/lib/$<CONFIG>/libmono-static-sgen.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})

add_subdirectory(${ENGINE_PATH}/script_core script_core)