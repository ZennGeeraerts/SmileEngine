/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_buffer.h"

namespace smile::graphic
{
    ConstantBuffer::ConstantBuffer( rhi::GPUBufferHandle handle, const ConstantBufferDescriptor &desc ) noexcept
        : m_Handle{ handle }, m_Descriptor{ desc }
    {
    }

    void ConstantBuffer::UpdateBuffer( const void *pBuffer, const Count size )
    {
        SM_ASSERT( size <= m_Descriptor.GetSize() );

        m_Buffer = pBuffer;
    }
}