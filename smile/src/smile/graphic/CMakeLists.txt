include(smile)

add_subdirectory(${THIRDPARTY_PATH}/imgui imgui)

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/imgui
    ${THIRDPARTY_PATH}/ImGuizmo
    ${THIRDPARTY_PATH}/assimp/include
)

set(LIB_LINKS
    imgui
    ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES
    *.cpp
    ${THIRDPARTY_PATH}/ImGuizmo/ImGuizmo.cpp
)
target_sources(smile PRIVATE ${SOURCE_FILES})