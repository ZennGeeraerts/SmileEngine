/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "render_engine.h"

#include "render_command.h"

#include "forward_renderer.h"
#include "wireframe_renderer.h"
#include "debug_renderer.h"
#include "renderer_2d.h"
#include "skybox_renderer.h"

#include <DirectXColors.h>

namespace smile::graphic
{
    ecs::RenderPassList RenderEngine::s_RenderPassList{};
    window::Window *RenderEngine::s_pWindow = nullptr;

    Ref< Framebuffer > RenderEngine::s_pFinalSceneFramebuffer{};

    RendererSettings RenderEngine::s_Settings{};
    ShaderLibrary RenderEngine::s_ShaderLibrary{};

    RenderEngine::CameraData RenderEngine::s_CameraData{};

    void RenderEngine::Initialize( window::Window *pWindow )
    {
        s_pWindow = pWindow;

        RenderCommand::Create( RendererAPI::API::DirectX11 );
        RenderCommand::Initalize( pWindow );

        BufferLayout bufferLayout{ { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float3, "NORMAL" } };
        s_ShaderLibrary.Load( "resources/shaders/PosColNorm.fx", bufferLayout );
        s_ShaderLibrary.Load( "resources/shaders/PosCol.fx", { { ShaderDataType::Float3, "POSITION" } } );
        s_ShaderLibrary.Load( "resources/shaders/PosColTex.fx",
            { { ShaderDataType::Float3, "POSITION" }, { ShaderDataType::Float2, "TEXCOORD" } } );
        s_ShaderLibrary.Load( "resources/shaders/Skybox.fx", { { ShaderDataType::Float3, "POSITION" } } );

        {
            FramebufferDescriptor frameBufferDesc{};
            frameBufferDesc.Attachments = { { FramebufferTextureFormat::RGBA8, true },
                FramebufferTextureFormat::Depth,
                { FramebufferTextureFormat::RGBA8, true } };
            frameBufferDesc.Width = s_Settings.Width;
            frameBufferDesc.Height = s_Settings.Height;
            frameBufferDesc.IsSwapChainTarget = false;

            s_pFinalSceneFramebuffer = RenderCommand::GetGraphicsDevice()->CreateFramebuffer( frameBufferDesc );
            s_pFinalSceneFramebuffer->ClearColor = { DirectX::Colors::DodgerBlue.f[0],
                DirectX::Colors::DodgerBlue.f[1],
                DirectX::Colors::DodgerBlue.f[2],
                DirectX::Colors::DodgerBlue.f[3] };
        }

        ForwardRenderer::Initialize();
        WireframeRenderer::GetInstance().Initialize();
        DebugRenderer::GetInstance().Initialize();
        Renderer2D::Initialize();
        SkyboxRenderer::Initialize();
    }

    void RenderEngine::ShutDown()
    {
        s_RenderPassList.ClearRenderPasses();

        ForwardRenderer::ShutDown();
        WireframeRenderer::GetInstance().ShutDown();
        DebugRenderer::GetInstance().ShutDown();
        Renderer2D::ShutDown();
        SkyboxRenderer::ShutDown();

        RenderCommand::ShutDown();
    }

    void RenderEngine::OnRender()
    {
        auto pContext = RenderCommand::GetGraphicsContext();
        pContext->ClearFramebuffer( s_pFinalSceneFramebuffer );
        pContext->BindFramebuffer( s_pFinalSceneFramebuffer );

        if ( s_CameraData.pMainCamera )
        {
            s_RenderPassList.OnRender( *s_CameraData.pMainCamera, s_CameraData.CameraTransform );

            SkyboxRenderer::BeginScene( *s_CameraData.pMainCamera, s_CameraData.CameraTransform );
            SkyboxRenderer::OnRender();
            SkyboxRenderer::EndScene();
        }

        pContext->UnbindFramebuffer();
    }

    void RenderEngine::OnRender( const EditorCamera &editorCamera )
    {
        GraphicsContext *pContext = RenderCommand::GetGraphicsContext();
        pContext->ClearFramebuffer( s_pFinalSceneFramebuffer );
        pContext->BindFramebuffer( s_pFinalSceneFramebuffer );

        s_RenderPassList.OnRender( editorCamera );

        SkyboxRenderer::BeginScene( editorCamera );
        SkyboxRenderer::OnRender();
        SkyboxRenderer::EndScene();

        pContext->UnbindFramebuffer();
    }

    void RenderEngine::OnWindowResize( Uint32 width, Uint32 height )
    {
        RenderCommand::ResizeWindow( 0, 0, width, height );
    }

    void RenderEngine::ResizeFramebuffer( Uint32 width, Uint32 height )
    {
        s_Settings.Width = width;
        s_Settings.Height = height;

        auto pDevice = RenderCommand::GetGraphicsDevice();
        pDevice->ResizeFramebuffer( s_pFinalSceneFramebuffer, width, height );
    }
}