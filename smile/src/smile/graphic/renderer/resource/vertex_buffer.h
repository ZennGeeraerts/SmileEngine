/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    class VertexBuffer final
    {
      public:
        VertexBuffer() = default;

        VertexBuffer( rhi::GPUBufferHandle handle, const rhi::BufferLayout &layout, const Count vertexCount ) noexcept
            : m_Handle{ handle }, m_BufferLayout{ layout }, m_VertexCount{ vertexCount }
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

        const rhi::BufferLayout &GetBufferLayout() const noexcept
        {
            return m_BufferLayout;
        }

        Count GetVertexCount() const noexcept
        {
            return m_VertexCount;
        }

        Uint32 GetStride() const noexcept
        {
            return m_BufferLayout.GetStride();
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        rhi::BufferLayout m_BufferLayout;
        Count m_VertexCount;

        friend class ResourceManager;
    };
}