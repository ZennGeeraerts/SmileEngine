/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_system.h"
#include "engine/graphic/renderer_backend/shader/shader_library.h"
#include "engine/graphic/camera/editor_camera.h"
#include "engine/graphic/scene/scene.h"

#include "window/window.h"

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
        static void Initialize( const window::Window *pWindow );
        static void ShutDown();

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
        static void SetCameraData( const CameraData &cameraData )
        {
            s_CameraData = cameraData;
        }

        static const CameraData &GetCameraData()
        {
            return s_CameraData;
        }

        static const RendererSettings &GetSettings()
        {
            return s_Settings;
        }
        static ShaderLibrary &GetShaderLibrary()
        {
            return s_ShaderLibrary;
        }

        static memory::Ref< Scene > GetScene()
        {
            return s_pScene;
        }

        static RenderSystem &GetRenderSystem()
        {
            return s_RenderSystem;
        }

      private:
        static RenderSystem s_RenderSystem;
        static memory::Ref< Scene > s_pScene;
        static const window::Window *s_pWindow;

        static RendererSettings s_Settings;
        static ShaderLibrary s_ShaderLibrary;

        static CameraData s_CameraData;
    };
}