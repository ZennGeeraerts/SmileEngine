include(smile)

set(INCLUDE_DIRS
    ${ENGINE_PATH}/../../thirdparty/dx11effects/include
    ${ENGINE_PATH}/../../thirdparty/dx11tex/include
)

set(LIB_LINKS
    d3d11
    dxgi
    dxguid
    d3dCompiler
    ${ENGINE_PATH}/../../thirdparty/dx11tex/x64/$<CONFIG>/DirectXTex.lib
    optimized ${ENGINE_PATH}/../../thirdparty/dx11effects/x64/dx11effects.lib
    debug ${ENGINE_PATH}/../../thirdparty/dx11effects/x64/dx11effects_d.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})