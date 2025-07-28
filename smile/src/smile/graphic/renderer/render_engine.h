/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_system.h"
#include "smile/graphic/scene/scene_manager.h"
#include "smile/graphic/renderer/shader/shader_library.h"

namespace smile::window
{
    class Window;
}

namespace smile::graphic
{
    class RenderEngine final
    {
      public:
        static void Initialize( const window::Window *pWindow );
        static void ShutDown();

        static void OnWindowResize( Uint32 width, Uint32 height );

        static RenderSystem &GetRenderSystem()
        {
            return s_RenderSystem;
        }

        static SceneManager &GetSceneManager()
        {
            return s_SceneManager;
        }

        static ShaderLibrary &GetShaderLibrary()
        {
            return s_ShaderLibrary;
        }

      private:
        static RenderSystem s_RenderSystem;
        static SceneManager s_SceneManager;
        static ShaderLibrary s_ShaderLibrary;
    };
}