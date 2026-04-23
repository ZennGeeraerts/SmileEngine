/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"

namespace smile::graphic
{
    class IndexBuffer final
    {
      public:
        IndexBuffer() = default;

        IndexBuffer( rhi::GPUBufferHandle handle, const Count indexCount ) noexcept
            : m_Handle{ handle }, m_IndexCount{ indexCount }
        {
        }

        IndexBuffer( const IndexBuffer & ) = default;
        IndexBuffer( IndexBuffer && ) noexcept = default;

        ~IndexBuffer() = default;

        IndexBuffer &operator=( const IndexBuffer & ) = default;
        IndexBuffer &operator=( IndexBuffer && ) noexcept = default;

        rhi::GPUBufferHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        Count GetIndexCount() const noexcept
        {
            return m_IndexCount;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        Count m_IndexCount;

        friend class ResourceManager;
    };
}