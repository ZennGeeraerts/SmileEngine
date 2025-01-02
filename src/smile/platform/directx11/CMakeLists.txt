include(smile-engine)

set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/thirdparty/dx11effects/include
    ${CMAKE_SOURCE_DIR}/thirdparty/dx11tex/include
)

set(LIB_LINKS
    d3d11
    dxgi
    dxguid
    d3dCompiler
    ${CMAKE_SOURCE_DIR}/thirdparty/dx11tex/x64/$<CONFIG>/DirectXTex.lib
    optimized ${CMAKE_SOURCE_DIR}/thirdparty/dx11effects/x64/dx11effects.lib
    debug ${CMAKE_SOURCE_DIR}/thirdparty/dx11effects/x64/dx11effects_d.lib
)

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})