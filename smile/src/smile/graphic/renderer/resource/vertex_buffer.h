/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/renderer_backend/render_handle.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"

namespace smile::graphic
{
    class VertexBuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< VertexBuffer >;

        VertexBuffer( GPUBufferHandle handle, const BufferLayout &layout, const Count vertexCount )
            : m_Handle{ handle }, m_BufferLayout{ layout }, m_VertexCount{ vertexCount }
        {
        }

        ~VertexBuffer() = default;

        const BufferLayout &GetBufferLayout() const
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
        GPUBufferHandle m_Handle;
        BufferLayout m_BufferLayout;
        Count m_VertexCount;

        friend class ResourceManager;
    };
}