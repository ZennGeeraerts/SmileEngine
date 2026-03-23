/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "renderer.h"

// #include "smile/graphic/scene/forward_renderer.h"
// #include "wireframe_renderer.h"
#include "debug_renderer.h"
// #include "renderer_2d.h"
// #include "smile/graphic/scene/skybox_renderer.h"

#include "smile/core/window/window.h"
#include "smile/core/world/world_manager.h"

namespace smile::graphic
{
    RenderSystem Renderer::s_RenderSystem{};
    RenderPassList Renderer::s_RenderPassList{};
    ShaderLibrary Renderer::s_ShaderLibrary{};

    Framebuffer::Ref Renderer::s_Framebuffer{};

    Uint32 Renderer::s_ViewportWidth;
    Uint32 Renderer::s_ViewportHeight;

    bool Renderer::s_RenderToSwapChain{ true };

    void Renderer::Initialize( const window::Window *pWindow )
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

        auto &resourceManager = s_RenderSystem.GetResourceManager();

        auto width = pWindow->GetWidth();
        auto height = pWindow->GetHeight();

        FramebufferAttachment colorAttachment = resourceManager.CreateColorAttachment( width, height );
        FramebufferAttachment depthAttachment = resourceManager.CreateDepthAttachment( width, height );
        FramebufferAttachment colorAttachment2 = resourceManager.CreateColorAttachment( width, height );

        s_Framebuffer = resourceManager.CreateFramebuffer( { colorAttachment, colorAttachment2 }, depthAttachment );
    }

    void Renderer::ShutDown()
    {
        s_RenderPassList.Clear();
    }

    void Renderer::OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform )
    {
        auto currentFramebuffer = s_RenderToSwapChain ? s_RenderSystem.GetBackBuffer() : s_Framebuffer;

        s_RenderSystem.Clear(
            currentFramebuffer, math::Color{ 0.392156899f, 0.584313750f, 0.929411829f, 1.0f }, 1.0f, 0.0f );

        s_RenderSystem.BeginFrame();
        s_RenderPassList.OnRender( currentFramebuffer, camera, cameraTransform );
        s_RenderSystem.EndFrame();
    }

    void Renderer::OnWindowResize( Uint32 width, Uint32 height )
    {
        s_RenderSystem.ResizeWindow( 0, 0, width, height );
    }

    void Renderer::OnViewportResize( Uint32 width, Uint32 height )
    {
        if ( width == s_ViewportWidth && height == s_ViewportHeight )
            return;

        s_ViewportWidth = width;
        s_ViewportWidth = height;

        auto &resourceManager = Renderer::GetRenderSystem().GetResourceManager();
        resourceManager.ResizeFramebuffer( s_Framebuffer, width, height );
    }
}