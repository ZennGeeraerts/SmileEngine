/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/render_handle.h"

namespace smile::graphic
{
    class Shader final
    {
      public:
        Shader( ShaderHandle handle ) : m_Handle{ handle }
        {
        }

      private:
        ShaderHandle m_Handle;
    };
}