include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

find_package(box2d REQUIRED)

set(INCLUDE_DIRS
    src
    thirdparty/imgui
    thirdparty/ImGuizmo
    thirdparty/assimp/include
    thirdparty/yaml-cpp/include
    thirdparty/mono/include
    thirdparty/PhysX/include
    thirdparty/PhysX/include/PhysX #For internal PhsysX usage
    thirdparty/dx11effects/include
    thirdparty/dx11tex/include
    thirdparty/sdl2_image-2.0.5/include
    thirdparty/sdl2-2.0.9/include
    ${CMAKE_SOURCE_DIR}/smile-raster/src
)

set(LIB_LINKS
    imgui
    smile-raster

    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/assimp/x64/assimp-vc140-mt.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/mono/lib/$<CONFIG>/libmono-static-sgen.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysX_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCharacterKinematic_static_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCommon_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXCooking_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXExtensions_static_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXFoundation_64.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/PhysX/lib/$<CONFIG>/PhysXPvdSDK_static_64.lib

    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/sdl2_image-2.0.5/x64/SDL2_image.lib
    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/sdl2-2.0.9/x64/SDL2.lib

    ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/dx11tex/x64/$<CONFIG>/DirectXTex.lib
)

if(WIN32)
    set(DX11_LIBS d3d11 dxgi dxguid d3dCompiler)
    set(WIN32_LIBS Ws2_32 Winmm Version Bcrypt)
else()
    set(DX11_LIBS)
    set(WIN32_LIBS)
endif()

file(GLOB_RECURSE SOURCE_FILES
    src/*.h
    src/*.cpp
    thirdparty/ImGuizmo/ImGuizmo.cpp
    thirdparty/ImGuizmo/ImGuizmo.h
)

add_library(smile-engine STATIC ${SOURCE_FILES})
target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})
target_link_libraries(smile-engine
    PRIVATE box2d::box2d
    PUBLIC ${LIB_LINKS} ${DX11_LIBS} ${WIN32_LIBS} 
    optimized ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/dx11effects/x64/dx11effects.lib 
    optimized ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/yaml-cpp/x64/yaml-cpp.lib
    debug ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/dx11effects/x64/dx11effects_d.lib 
    debug ${CMAKE_SOURCE_DIR}/smile-engine/thirdparty/yaml-cpp/x64/yaml-cppd.lib)

target_precompile_headers(smile-engine PUBLIC "src/smpch.h")

GroupSourcesByFolder(smile-engine)

set_property(TARGET smile-engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(smile-engine PROPERTIES FOLDER Core)

target_compile_definitions(
    smile-engine
    PUBLIC
    $<$<CONFIG:Debug>:SM_C_DEBUG>
    $<$<CONFIG:Release>:SM_C_RELEASE>
)

add_definitions(-DUNICODE)