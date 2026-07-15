/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    class VertexBuffer final
    {
      public:
        VertexBuffer() = default;

        VertexBuffer( rhi::GPUBufferHandle handle, const Count vertexCount, const Uint32 stride ) noexcept
            : m_Handle{ handle }, m_VertexCount{ vertexCount }, m_Stride{ stride }
        {
        }

        VertexBuffer( const VertexBuffer & ) = default;
        VertexBuffer( VertexBuffer && ) noexcept = default;

        ~VertexBuffer() = default;

        VertexBuffer &operator=( const VertexBuffer & ) = default;
        VertexBuffer &operator=( VertexBuffer && ) = default;

        rhi::GPUBufferHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        Count GetVertexCount() const noexcept
        {
            return m_VertexCount;
        }

        Uint32 GetStride() const noexcept
        {
            return m_Stride;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        bool operator==( const VertexBuffer &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        Count m_VertexCount = 0;
        Uint32 m_Stride = 0;

        friend class ResourceManager;
    };
}