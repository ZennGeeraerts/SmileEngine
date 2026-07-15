/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/shader/shader.h"

namespace smile::graphic
{
    class VertexShader final
    {
      public:
        VertexShader() = default;

        VertexShader( rhi::ShaderHandle handle ) noexcept : m_Handle{ handle }
        {
        }

        VertexShader( const VertexShader & ) = default;
        VertexShader( VertexShader && ) noexcept = default;
        ~VertexShader() = default;

        VertexShader &operator=( const VertexShader & ) = default;
        VertexShader &operator=( VertexShader && ) noexcept = default;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        rhi::ShaderHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        bool operator==( const VertexShader &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::ShaderHandle m_Handle;

        friend class ResourceManager;
    };
}