include(smile-engine)

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparty/imgui imgui)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/ImGuizmo
    ${CMAKE_SOURCE_DIR}/thirdparty/assimp/include
)

set(LIB_LINKS
    imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/assimp/x64/assimp-vc140-mt.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES
    *.cpp
    ${CMAKE_SOURCE_DIR}/thirdparty/ImGuizmo/ImGuizmo.cpp
)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})