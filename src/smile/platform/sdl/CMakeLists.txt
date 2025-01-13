include(smile)

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/sdl2_image-2.0.5/include
    ${THIRDPARTY_PATH}/sdl2-2.0.9/include
)

set(LIB_LINKS
    ${THIRDPARTY_PATH}/sdl2_image-2.0.5/x64/SDL2_image.lib
    ${THIRDPARTY_PATH}/sdl2-2.0.9/x64/SDL2.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE SOURCE_FILES)