/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "constant_buffer.h"

namespace smile::graphic
{
    ConstantBuffer::ConstantBuffer( rhi::GPUBufferHandle handle, const ConstantBufferDescriptor &desc )
        : m_Handle{ handle }, m_pBuffer{ nullptr }, m_Descriptor{ desc }
    {
    }

    void ConstantBuffer::InitializeBuffer( void *pBuffer, const Count size )
    {
        SM_ASSERT( size <= m_Descriptor.GetSize() );
        m_pBuffer = pBuffer;
    }
}