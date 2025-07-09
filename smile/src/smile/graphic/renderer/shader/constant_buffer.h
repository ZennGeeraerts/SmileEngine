/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"
#include "smile/graphic/rhi/render_handle.h"
#include "constant_buffer_descriptor.h"

namespace smile::graphic
{
    class ConstantBuffer final : public memory::Counted
    {
      public:
        using Ref = memory::Ref< ConstantBuffer >;

        ConstantBuffer( GPUBufferHandle handle, const ConstantBufferDescriptor &desc );
        ~ConstantBuffer() = default;

        template < typename ConstantBufferType >
        void Initialize( ConstantBufferType *pBuffer )
        {
            InitializeBuffer( pBuffer, sizeof( ConstantBufferType ) );
        }

        bool IsValid() const
        {
            return m_Handle.IsValid();
        }

        const void *GetBuffer() const
        {
            return m_pBuffer;
        }

      private:
        void InitializeBuffer( const void *pBuffer, const Count size );

      private:
        GPUBufferHandle m_Handle;
        const void *m_pBuffer;
        ConstantBufferDescriptor m_Descriptor;

        friend class ResourceManager;
    };
}