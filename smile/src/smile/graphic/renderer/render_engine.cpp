/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_engine.h"

#include "smile/core/window/window.h"

namespace smile::graphic
{
    RenderContext RenderEngine::s_RenderContext{};
    ShaderLibrary RenderEngine::s_ShaderLibrary{};
    MaterialSystem RenderEngine::s_MaterialSystem{};
    Renderer RenderEngine::s_Renderer{};

    void RenderEngine::Initialize( const window::Window *pWindow )
    {
        s_RenderContext.Initialize( pWindow );

        s_ShaderLibrary.LoadShader( "resources/shaders/debug_renderer.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pos_col.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pos_tex.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/col_tex.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/skybox.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/skybox.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr.vs.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr.ps.smshader" );
        s_ShaderLibrary.LoadShader( "resources/shaders/pbr_skinned.vs.smshader" );
    }

    void RenderEngine::ShutDown()
    {
    }
}