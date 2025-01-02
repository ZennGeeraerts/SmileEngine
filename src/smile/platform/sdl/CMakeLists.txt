include(smile-engine)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/thirdparty/sdl2_image-2.0.5/include
    ${CMAKE_SOURCE_DIR}/thirdparty/sdl2-2.0.9/include
)

set(LIB_LINKS
    ${CMAKE_SOURCE_DIR}/thirdparty/sdl2_image-2.0.5/x64/SDL2_image.lib
    ${CMAKE_SOURCE_DIR}/thirdparty/sdl2-2.0.9/x64/SDL2.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE SOURCE_FILES)