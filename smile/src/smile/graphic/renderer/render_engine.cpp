/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_engine.h"

#include "forward_renderer.h"
#include "wireframe_renderer.h"
#include "debug_renderer.h"
#include "renderer_2d.h"
#include "skybox_renderer.h"

#include "smile/core/window/window.h"
#include "smile/core/world/world_manager.h"

namespace smile::graphic
{
    RenderSystem RenderEngine::s_RenderSystem{};
    SceneManager RenderEngine::s_SceneManager{};
    ShaderLibrary RenderEngine::s_ShaderLibrary{};

    void RenderEngine::Initialize( const window::Window *pWindow )
    {
        s_RenderSystem.Initialize( pWindow );

        BufferLayout vertexLayout{ { Format::RGB32_FLOAT, "POSITION" }, { Format::RGB32_FLOAT, "NORMAL" } };
        s_ShaderLibrary.Load( "resources/shaders/PosColNorm.fx", vertexLayout );
        s_ShaderLibrary.Load( "resources/shaders/PosCol.fx", { { Format::RGB32_FLOAT, "POSITION" } } );
        s_ShaderLibrary.Load( "resources/shaders/PosColTex.fx",
            { { Format::RGB32_FLOAT, "POSITION" }, { Format::RG32_FLOAT, "TEXCOORD" } } );
        s_ShaderLibrary.Load( "resources/shaders/Skybox.fx", { { Format::RGB32_FLOAT, "POSITION" } } );

        s_SceneManager.Initialize( pWindow );
        world::WorldManager::AddListener( &s_SceneManager );

        ForwardRenderer::Initialize();
        WireframeRenderer::GetInstance().Initialize();
        DebugRenderer::GetInstance().Initialize();
        Renderer2D::Initialize();
        SkyboxRenderer::Initialize();
    }

    void RenderEngine::ShutDown()
    {
        ForwardRenderer::ShutDown();
        WireframeRenderer::GetInstance().ShutDown();
        DebugRenderer::GetInstance().ShutDown();
        Renderer2D::ShutDown();
        SkyboxRenderer::ShutDown();
    }

    void RenderEngine::OnWindowResize( Uint32 width, Uint32 height )
    {
        s_RenderSystem.ResizeWindow( 0, 0, width, height );
    }
}