/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_engine.h"

// #include "smile/graphic/scene/forward_renderer.h"
// #include "wireframe_renderer.h"
#include "debug_renderer.h"
// #include "renderer_2d.h"
// #include "smile/graphic/scene/skybox_renderer.h"

#include "smile/core/window/window.h"
#include "smile/core/world/world_manager.h"

namespace smile::graphic
{
    RenderSystem RenderEngine::s_RenderSystem{};
    ShaderLibrary RenderEngine::s_ShaderLibrary{};

    void RenderEngine::Initialize( const window::Window *pWindow )
    {
        s_RenderSystem.Initialize( pWindow );

        s_ShaderLibrary.LoadShader( "resources/shaders/debug_renderer.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pos_col.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pos_tex.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/col_tex.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/skybox.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/skybox.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr_skinned.vs.smshader" );

        DebugRenderer::GetInstance().Initialize();
    }

    void RenderEngine::ShutDown()
    {
        DebugRenderer::GetInstance().ShutDown();
    }
}