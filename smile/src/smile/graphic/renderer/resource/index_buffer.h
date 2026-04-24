/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/resource/buffer.h"

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

        bool operator==( const IndexBuffer &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        Count m_IndexCount = 0;

        friend class ResourceManager;
    };
}