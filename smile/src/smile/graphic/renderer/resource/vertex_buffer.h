/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    class VertexBuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< VertexBuffer >;
        using ConstRef = memory::Ref< const VertexBuffer >;

        VertexBuffer( rhi::GPUBufferHandle handle, const rhi::BufferLayout &layout, const Count vertexCount )
            : m_Handle{ handle }, m_BufferLayout{ layout }, m_VertexCount{ vertexCount }
        {
        }

        ~VertexBuffer() = default;

        rhi::GPUBufferHandle GetHandle() const
        {
            return m_Handle;
        }

        const rhi::BufferLayout &GetBufferLayout() const
        {
            return m_BufferLayout;
        }

        Count GetVertexCount() const
        {
            return m_VertexCount;
        }

        Uint32 GetStride() const
        {
            return m_BufferLayout.GetStride();
        }

        bool IsValid() const
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