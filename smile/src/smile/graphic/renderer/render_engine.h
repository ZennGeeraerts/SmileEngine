/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_system.h"
#include "smile/graphic/renderer_backend/shader/shader_library.h"
#include "smile/graphic/camera/editor_camera.h"
#include "smile/graphic/scene/scene.h"

#include "smile/core/window/window.h"

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

        static void SetSettings( const RendererSettings &settings )
        {
            s_Settings = settings;
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
    };
}