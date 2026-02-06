/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class PixelShader final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< PixelShader >;
        using ConstRef = memory::Ref< const PixelShader >;

        PixelShader( rhi::ShaderHandle handle ) : m_Handle{ handle }
        {
        }

        ~PixelShader() = default;

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::ShaderHandle m_Handle;

        friend class ResourceManager;
    };
}