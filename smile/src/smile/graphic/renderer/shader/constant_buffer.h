/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/render_handle.h"
#include "smile/graphic/shader/constant_buffer_descriptor.h"

namespace smile::graphic
{
    class ConstantBuffer final
    {
      public:
        ConstantBuffer() = default;
        ConstantBuffer( rhi::GPUBufferHandle handle, const ConstantBufferDescriptor &desc ) noexcept;

        ConstantBuffer( const ConstantBuffer & ) = default;
        ConstantBuffer( ConstantBuffer && ) noexcept = default;
        ~ConstantBuffer() = default;

        ConstantBuffer &operator=( const ConstantBuffer & ) = default;
        ConstantBuffer &operator=( ConstantBuffer && ) noexcept = default;

        template < typename ConstantBufferType >
        void Update( const ConstantBufferType *pBuffer )
        {
            UpdateBuffer( pBuffer, sizeof( ConstantBufferType ) );
        }

        void UpdateBuffer( const void *pBuffer, const Count size );

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        const void *GetBuffer() const noexcept
        {
            return m_Buffer;
        }

        rhi::GPUBufferHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        const ConstantBufferDescriptor &GetDescriptor() const noexcept
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