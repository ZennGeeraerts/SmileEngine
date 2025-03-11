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

#include "resource/resource_manager.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    RenderSystem RenderEngine::s_RenderSystem{};
    const window::Window *RenderEngine::s_pWindow = nullptr;

    memory::Ref< Scene > RenderEngine::s_pScene = nullptr;

    RendererSettings RenderEngine::s_Settings{};
    ShaderLibrary RenderEngine::s_ShaderLibrary{};

    RenderEngine::CameraData RenderEngine::s_CameraData{};

    void RenderEngine::Initialize( const window::Window *pWindow )
    {
        s_pWindow = pWindow;
        s_RenderSystem.Initialize( pWindow );

        BufferLayout bufferLayout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float3, "NORMAL" } };
        s_ShaderLibrary.Load( "resources/shaders/PosColNorm.fx", bufferLayout );
        s_ShaderLibrary.Load( "resources/shaders/PosCol.fx", { { ShaderDataType::Float3, "POSITION" } } );
        s_ShaderLibrary.Load( "resources/shaders/PosColTex.fx",
            { { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float2, "TEXCOORD" } } );
        s_ShaderLibrary.Load( "resources/shaders/Skybox.fx", { { ShaderDataType::Float3, "POSITION" } } );

        s_pScene = memory::CreateRef< Scene >( pWindow );

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

    void RenderEngine::ResizeFramebuffer( Uint32 width, Uint32 height )
    {
        s_Settings.Width = width;
        s_Settings.Height = height;

        auto &resourceManager = s_RenderSystem.GetResourceManager();
        resourceManager.ResizeFramebuffer( s_pScene->GetFramebuffer(), width, height );
    }
}