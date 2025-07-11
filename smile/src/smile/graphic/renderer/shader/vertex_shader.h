/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class VertexShader final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< VertexShader >;
        using ConstRef = memory::Ref< const VertexShader >;

        VertexShader( ShaderHandle handle ) : m_Handle{ handle }
        {
        }

        ~VertexShader() = default;

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        ShaderHandle m_Handle;

        friend class ResourceManager;
    };
}