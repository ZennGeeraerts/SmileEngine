include(smile)

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/mono/include
)

set(LIB_LINKS
    ${THIRDPARTY_PATH}/mono/lib/$<CONFIG>/libmono-static-sgen.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})

add_subdirectory(${SMILE_PATH}/../../smile-script-core script_core)