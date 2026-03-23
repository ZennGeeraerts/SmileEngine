/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_system.h"
#include "render_pass_list.h"
#include "smile/graphic/shader/shader_library.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class Renderer final
    {
      public:
        static void Initialize( const window::Window *pWindow );
        static void ShutDown();

        static void OnRender( const Camera &camera, const DirectX::XMFLOAT4X4 &cameraTransform );

        static void OnWindowResize( Uint32 width, Uint32 height );
        static void OnViewportResize( Uint32 width, Uint32 height );

        static RenderSystem &GetRenderSystem()
        {
            return s_RenderSystem;
        }

        static RenderPassList &GetRenderPassList()
        {
            return s_RenderPassList;
        }

        static ShaderLibrary &GetShaderLibrary()
        {
            return s_ShaderLibrary;
        }

      private:
        static RenderSystem s_RenderSystem;
        static RenderPassList s_RenderPassList;
        static ShaderLibrary s_ShaderLibrary;

        static Framebuffer::Ref s_Framebuffer;

        static Uint32 s_ViewportWidth;
        static Uint32 s_ViewportHeight;

        static bool s_RenderToSwapChain;
    };
}