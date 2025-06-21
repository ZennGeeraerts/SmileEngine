/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/renderer_backend/render_handle.h"
#include "smile/graphic/renderer_backend/resource/buffer.h"

namespace smile::graphic
{
    class ConstantBuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< ConstantBuffer >;

        ConstantBuffer( GPUBufferHandle handle, const BufferLayout &layout )
            : m_Handle{ handle }, m_BufferLayout{ layout }
        {
        }

        ~ConstantBuffer() = default;

        const BufferLayout &GetBufferLayout() const
        {
            return m_BufferLayout;
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

      private:
        GPUBufferHandle m_Handle;
        BufferLayout m_BufferLayout;

        friend class ResourceManager;
    };
}