add_subdirectory(${ENGINE_PATH}/smile/raster)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/smile/raster/src
)

set(LIB_LINKS
    smile-raster
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE SOURCE_FILES)