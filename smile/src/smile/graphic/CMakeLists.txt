include(smile)

add_subdirectory(${THIRDPARTY_PATH}/imgui imgui)

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/imgui
    ${THIRDPARTY_PATH}/ImGuizmo
    ${THIRDPARTY_PATH}/assimp/include
)

find_package(stb REQUIRED)

set(LIB_LINKS
    imgui
    ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.lib
    stb::stb
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

include(build/compile_shaders.cmake)
include(build/rhi.cmake)