/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class PixelShader final
    {
      public:
        PixelShader() = default;

        PixelShader( rhi::ShaderHandle handle ) noexcept : m_Handle{ handle }
        {
        }

        PixelShader( const PixelShader & ) = default;
        PixelShader( PixelShader && ) noexcept = default;
        ~PixelShader() = default;

        PixelShader &operator=( const PixelShader & ) = default;
        PixelShader &operator=( PixelShader && ) noexcept = default;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        rhi::ShaderHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        bool operator==( const PixelShader &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::ShaderHandle m_Handle;

        friend class ResourceManager;
    };
}