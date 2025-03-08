cmake_minimum_required(VERSION "3.25.0")

get_filename_component(SMILE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../smile/src" ABSOLUTE)
get_filename_component(SMILEDIT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../../smiledit" ABSOLUTE)
get_filename_component(SCRIPT_CORE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../../smile-script-core" ABSOLUTE)
message("PATH: " ${SMILE_PATH})

file(GLOB_RECURSE ENGINE_SOURCE_FILES "${SCRIPT_CORE_PATH}/Source/*.cs")
project("smile-script-core" LANGUAGES CSharp)
add_library("smile-script-core" SHARED ${ENGINE_SOURCE_FILES})

set_target_properties("smile-script-core"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${SMILE_PATH}/../../resources/scripts"
    LIBRARY_OUTPUT_DIRECTORY "${SMILE_PATH}/../../resources/scripts"
    RUNTIME_OUTPUT_DIRECTORY "${SMILE_PATH}/../../resources/scripts"
)

file(GLOB_RECURSE APP_SOURCE_FILES Source/*.cs)
project("sandbox" LANGUAGES CSharp)
add_library("sandbox" SHARED ${APP_SOURCE_FILES})
target_link_libraries("sandbox" smile-script-core)

set_target_properties("sandbox"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
)

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../worlds"
        "${SMILEDIT_PATH}/out/build/x64-$<CONFIG>/$<CONFIG>/sandbox-project/assets/worlds"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../textures"
        "${SMILEDIT_PATH}/out/build/x64-$<CONFIG>/$<CONFIG>/sandbox-project/assets/textures"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../meshes"
        "${SMILEDIT_PATH}/out/build/x64-$<CONFIG>/$<CONFIG>/sandbox-project/assets/meshes"
    )
    
add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/../shaders"
        "${SMILEDIT_PATH}/out/build/x64-$<CONFIG>/$<CONFIG>/sandbox-project/assets/shaders"
    )

add_custom_command(
    TARGET sandbox
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_LIST_DIR}/bin"
        "${SMILEDIT_PATH}/out/build/x64-$<CONFIG>/$<CONFIG>/sandbox-project/assets/scripts/bin"
    )