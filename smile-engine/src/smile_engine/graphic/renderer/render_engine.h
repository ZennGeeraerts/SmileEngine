/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "ecs/render_pass_list.h"
#include "smile_engine/graphic/renderer_api/renderer_api.h"

#include "smile_engine/graphic/renderer_api/shader/shader_library.h"
#include "smile_engine/graphic/renderer_api/resource/frame_buffer.h"

#include "smile_engine/core/window/window.h"

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
        static void Initialize( window::Window *pWindow );
        static void ShutDown();

        static void OnRender();
        static void OnRender( const EditorCamera &editorCamera );

        static void AddRenderPass( ecs::RenderPass *pRenderPass )
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

        struct CameraData final
        {
            Camera *pMainCamera = nullptr;
            DirectX::XMFLOAT4X4 CameraTransform{};
        };
        static void SetCameraData(const CameraData& cameraData)
        {
            s_CameraData = cameraData;
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
        static ecs::RenderPassList s_RenderPassList;
        static window::Window *s_pWindow;

        static Ref< Framebuffer > s_pFinalSceneFramebuffer;

        static RendererSettings s_Settings;
        static ShaderLibrary s_ShaderLibrary;

        static CameraData s_CameraData;
    };
}