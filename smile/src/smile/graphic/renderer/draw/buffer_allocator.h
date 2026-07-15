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
 * @file        buffer_allocator.h
 * @author      Zenn Geeraerts
 * @created     2 July 2026
 * @brief       Buffer allocator for managing GPU buffer slices.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/array.h"
#include "smile/graphic/rhi/resource/buffer.h"

namespace smile::graphic
{
    class ResourceManager;

    struct BufferSlice
    {
        rhi::GPUBufferHandle Buffer;
        rhi::BufferRange Range;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return Buffer.IsValid();
        }
    };

    class BufferAllocator final
    {
      public:
        BufferAllocator( ResourceManager &resourceManager, const rhi::GPUBufferDescriptor &desc ) noexcept;

        void Initialize( Count initialCapacity );

        BufferSlice Allocate( const Count size, Uint32 alignment );

        void Reset() noexcept;

      private:
        void Grow( Count newCapacity );
        void CreateBuffers( const Count capacity );

        static constexpr Count s_MaxBufferedFrames = 3;

        primitive::Array< rhi::GPUBufferHandle, s_MaxBufferedFrames > m_Buffers;
        rhi::GPUBufferDescriptor m_Descriptor;

        Index m_CurrentBuffer{ 0 };
        Count m_Offset{ 0 };

        ResourceManager &m_ResourceManager;
    };
}