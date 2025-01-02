include(smile-engine)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/thirdparty/mono/include
)

set(LIB_LINKS
    ${CMAKE_SOURCE_DIR}/thirdparty/mono/lib/$<CONFIG>/libmono-static-sgen.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})

add_subdirectory(${ENGINE_PATH}/smile/script_core script_core)