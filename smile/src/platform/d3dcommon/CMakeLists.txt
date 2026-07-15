include(smile)

set(INCLUDE_DIRS
    ${THIRDPARTY_PATH}/dx11tex/include
)

set(LIB_LINKS
    dxgi
    dxguid
    d3dCompiler
    ${THIRDPARTY_PATH}/dx11tex/x64/$<CONFIG>/DirectXTex.lib
)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})