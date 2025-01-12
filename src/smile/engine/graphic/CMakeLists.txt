include(smile)

add_subdirectory(${ENGINE_PATH}/../../thirdparty/imgui imgui)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/../../thirdparty/imgui
    ${ENGINE_PATH}/../../thirdparty/ImGuizmo
    ${ENGINE_PATH}/../../thirdparty/assimp/include
)

set(LIB_LINKS
    imgui
    ${ENGINE_PATH}/../../thirdparty/assimp/x64/assimp-vc140-mt.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES
    *.cpp
    ${ENGINE_PATH}/../../thirdparty/ImGuizmo/ImGuizmo.cpp
)
target_sources(smile PRIVATE ${SOURCE_FILES})