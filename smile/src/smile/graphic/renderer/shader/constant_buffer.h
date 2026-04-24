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
        ConstantBuffer( rhi::GPUBufferHandle handle, const Count size ) noexcept;

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

        Count GetSize() const noexcept
        {
            return m_Size;
        }

        bool operator==( const ConstantBuffer &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::GPUBufferHandle m_Handle;
        const void *m_Buffer = nullptr;
        Count m_Size;

        friend class ResourceManager;
    };
}