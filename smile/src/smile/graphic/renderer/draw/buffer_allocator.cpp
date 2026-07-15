/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        buffer_allocator.cpp
 * @author      Zenn Geeraerts
 * @created     2 July 2026
 * @brief       Buffer allocator for managing GPU buffer slices.
 */
#include "smpch.h"
#include "buffer_allocator.h"

#include "smile/common/memory/memory.h"
#include "smile/graphic/renderer/resource/resource_manager.h"

namespace smile::graphic
{
    BufferAllocator::BufferAllocator( ResourceManager &resourceManager, const rhi::GPUBufferDescriptor &desc ) noexcept
        : m_ResourceManager{ resourceManager }, m_Descriptor{ desc }
    {
    }

    void BufferAllocator::Initialize( Count initialCapacity )
    {
        initialCapacity = memory::Align( initialCapacity, 256u );
        CreateBuffers( initialCapacity );
    }

    void BufferAllocator::Reset() noexcept
    {
        m_CurrentBuffer = ( m_CurrentBuffer + 1 ) % s_MaxBufferedFrames;
        m_Offset = 0;
    }

    BufferSlice BufferAllocator::Allocate( const Count size, Uint32 alignment )
    {
        alignment = std::max( alignment, 1u );

        Count offset = memory::Align( m_Offset, alignment );

        auto end = offset + size;

        if ( end > m_Descriptor.Size )
        {
            Grow( end );

            offset = memory::Align( m_Offset, alignment );
            end = offset + size;
        }

        const rhi::BufferRange range{ offset, size };
        const BufferSlice slice{ m_Buffers[m_CurrentBuffer], range };

        m_Offset = end;

        return slice;
    }

    void BufferAllocator::Grow( Count newCapacity )
    {
        newCapacity = std::max( newCapacity, m_Descriptor.Size * 2 );
        newCapacity = memory::Align( newCapacity, 256u );

        CreateBuffers( newCapacity );
    }

    void BufferAllocator::CreateBuffers( const Count capacity )
    {
        m_Descriptor.Size = capacity;

        for ( auto &buffer : m_Buffers )
        {
            if ( buffer.IsValid() )
            {
                m_ResourceManager.DestroyGPUBuffer( buffer );
            }

            buffer = m_ResourceManager.CreateGPUBuffer( m_Descriptor );
        }
    }
}