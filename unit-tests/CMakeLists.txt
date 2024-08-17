include(${CMAKE_SOURCE_DIR}/scripts/cmake_utils.cmake)

set(INCLUDE_DIRS src thirdparty ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h thirdparty/catch.hpp)

add_executable(unit-tests ${SOURCE_FILES})
target_link_libraries(unit-tests PRIVATE smile-engine)
target_include_directories(unit-tests PRIVATE ${INCLUDE_DIRS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)

group_sources_by_folder(unit-tests)
set_property(TARGET unit-tests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(unit-tests PROPERTIES FOLDER Tools)

add_custom_command(
    TARGET unit-tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET unit-tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET unit-tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )

add_custom_command(
    TARGET unit-tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.dll
        ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>
    )