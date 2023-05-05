/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "renderer/render_pass/render_pass_list.h"
#include "renderer_api.h"

#include "shader/shader_library.h"
#include "resource/frame_buffer.h"

#include "smile_engine/core/window.h"

namespace smile::graphic
{
    struct RendererSettings final
    {
        Uint32 Width = 1280;
        Uint32 Height = 720;
    };

    class RenderEngine final
    {
      public:
        static void Initialize( Window *pWindow );
        static void ShutDown();

        static void OnRender();
        static void OnRender( const EditorCamera &editorCamera );

        static void AddRenderPass( RenderPass *pRenderPass )
        {
            s_RenderPassList.AddRenderPass( pRenderPass );
        }
        static void ClearRenderPasses()
        {
            s_RenderPassList.ClearRenderPasses();
        }

        static void OnWindowResize( Uint32 width, Uint32 height );
        static void ResizeFramebuffer( Uint32 width, Uint32 height );

        static void SetSettings( const RendererSettings &settings )
        {
            s_Settings = settings;
        }

        static GraphicsDevice* GetDevice()
        {
            return s_pWindow->GetGraphicsDevice();
        }
        static GraphicsContext *GetContext()
        {
            return s_pWindow->GetGraphicsContext();
        }
        static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }
        static const RendererSettings &GetSettings()
        {
            return s_Settings;
        }
        static ShaderLibrary &GetShaderLibrary()
        {
            return s_ShaderLibrary;
        }
        static void *GetFinalColor()
        {
            return s_pFinalSceneFramebuffer->GetColor( 0 );
        }

      private:
        static RenderPassList s_RenderPassList;
        static Window *s_pWindow;

        static Ref< Framebuffer > s_pFinalSceneFramebuffer;

        static RendererSettings s_Settings;
        static ShaderLibrary s_ShaderLibrary;
    };
}