include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

set(INCLUDE_DIRS src ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(smile-editor ${SOURCE_FILES})
target_link_libraries(smile-editor PRIVATE smile-engine)
target_include_directories(smile-editor PRIVATE ${INCLUDE_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/mono"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/mono"
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

# Copy the dlls to the binary directory
add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-editor
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

GroupSourcesByFolder(smile-editor)
set_property(TARGET smile-editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties (smile-editor PROPERTIES FOLDER Tools)