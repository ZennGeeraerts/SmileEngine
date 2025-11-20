include(smile)

if(COMMAND add_serializer)
    add_serializer(graphic)
endif()

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/assimp/include
)

set(LIB_LINKS
    ${THIRDPARTY_PATH}/assimp/x64/assimp-vc140-mt.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

include(build/rhi.cmake)
include(build/shader.cmake)
include(build/tools.cmake)
include(build/compile_shaders.cmake)
include(build/resource.cmake)
include(build/renderer.cmake)
include(build/sprite.cmake)
include(build/animation.cmake)
include(build/imgui.cmake)
include(build/scene.cmake)