include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

set(INCLUDE_DIRS src ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)

add_executable(smile-project ${SOURCE_FILES})
target_link_libraries(smile-project PRIVATE smile-engine)
target_include_directories(smile-project PRIVATE ${INCLUDE_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/assets"
    )

# Copy the dlls to the binary directory
add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/assimp/x64/assimp-vc140-mt.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET smile-project
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

GroupSourcesByFolder(smile-project)
set_property(TARGET smile-project PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties (smile-project PROPERTIES FOLDER Misc)