/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/shader/constant_buffer_descriptor.h"

namespace smile::graphic
{
    class ConstantBuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< ConstantBuffer >;

        ConstantBuffer( rhi::GPUBufferHandle handle, const ConstantBufferDescriptor &desc );
        ~ConstantBuffer() = default;

        template < typename ConstantBufferType >
        void Update( const ConstantBufferType *pBuffer )
        {
            UpdateBuffer( pBuffer, sizeof( ConstantBufferType ) );
        }

        void UpdateBuffer( const void *pBuffer, const Count size );

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

        const void *GetBuffer() const
        {
            return m_Buffer;
        }

        rhi::GPUBufferHandle GetHandle() const
        {
            return m_Handle;
        }

        const ConstantBufferDescriptor &GetDescriptor() const
        {
            return m_Descriptor;
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        const void *m_Buffer = nullptr;
        ConstantBufferDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}