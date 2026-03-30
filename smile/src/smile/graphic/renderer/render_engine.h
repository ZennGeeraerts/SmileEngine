/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "render_context.h"
#include "smile/graphic/shader/shader_library.h"
#include "material/material_system.h"

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

        static RenderContext &GetRenderContext()
        {
            return s_RenderContext;
        }

        static ShaderLibrary &GetShaderLibrary()
        {
            return s_ShaderLibrary;
        }

        static MaterialSystem &GetMaterialSystem()
        {
            return s_MaterialSystem;
        }

      private:
        static RenderContext s_RenderContext;
        static ShaderLibrary s_ShaderLibrary;
        static MaterialSystem s_MaterialSystem;
    };
}