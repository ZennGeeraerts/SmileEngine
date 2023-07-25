cmake_minimum_required(VERSION "3.20.0")

file(GLOB_RECURSE ENGINE_SOURCE_FILES ../../../../smile-script-core/Source/*.cs)
project("smile-script-core" LANGUAGES CSharp)
add_library("smile-script-core" SHARED ${ENGINE_SOURCE_FILES})

set_target_properties("smile-script-core"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../../../smile-editor/resources/scripts"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../../../smile-editor/resources/scripts"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../../../smile-editor/resources/scripts"
)

file(GLOB_RECURSE APP_SOURCE_FILES Source/*.cs)
project("sandbox" LANGUAGES CSharp)
add_library("sandbox" SHARED ${APP_SOURCE_FILES})
target_link_libraries("sandbox" smile-script-core)

set_target_properties("sandbox"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/../../../sandbox-project/assets/scenes"
        "${CMAKE_SOURCE_DIR}/../../../../out/build/x64-$<CONFIG>/smile-editor/$<CONFIG>/sandbox-project/assets/scenes"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/../../../sandbox-project/assets/textures"
        "${CMAKE_SOURCE_DIR}/../../../../out/build/x64-$<CONFIG>/smile-editor/$<CONFIG>/sandbox-project/assets/textures"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/../../../sandbox-project/assets/meshes"
        "${CMAKE_SOURCE_DIR}/../../../../out/build/x64-$<CONFIG>/smile-editor/$<CONFIG>/sandbox-project/assets/meshes"
    )
    
add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/../../../sandbox-project/assets/shaders"
        "${CMAKE_SOURCE_DIR}/../../../../out/build/x64-$<CONFIG>/smile-editor/$<CONFIG>/sandbox-project/assets/shaders"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/../../../sandbox-project/assets/scripts/bin"
        "${CMAKE_SOURCE_DIR}/../../../../out/build/x64-$<CONFIG>/smile-editor/$<CONFIG>/sandbox-project/assets/scripts/bin"
    )