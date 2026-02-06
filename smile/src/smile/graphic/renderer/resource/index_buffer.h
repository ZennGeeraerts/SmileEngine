/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class IndexBuffer : public memory::Counted
    {
      public:
        using Ref = memory::Ref< IndexBuffer >;

        IndexBuffer( rhi::GPUBufferHandle handle, const Count indexCount )
            : m_Handle{ handle }, m_IndexCount{ indexCount }
        {
        }

        ~IndexBuffer() = default;

        rhi::GPUBufferHandle GetHandle() const
        {
            return m_Handle;
        }

        Count GetIndexCount() const
        {
            return m_IndexCount;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        Count m_IndexCount;

        friend class ResourceManager;
    };
}