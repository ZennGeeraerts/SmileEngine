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
 * @file        frame_arena.cpp
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       FrameArena implementation — bump-pointer arena with chunked growth.
 */
#include "smpch.h"
#include "frame_arena.h"

#include "smile/common/memory/memory.h"

#include <malloc.h>

namespace smile::graphic
{
    FrameArena::FrameArena( const Count initialChunkSize ) noexcept : m_InitialChunkSize{ initialChunkSize }
    {
    }

    FrameArena::~FrameArena()
    {
        ReleaseChunks();
    }

    Byte *FrameArena::AllocateBytes( const Count size, const Count alignment )
    {
        SM_ASSERT( size > 0 );
        SM_ASSERT( alignment > 0 && ( alignment & ( alignment - 1 ) ) == 0 ); // power of two

        if ( m_CurrentChunkIndex == s_InvalidIndex )
        {
            AddChunk( size );
        }

        while ( true )
        {
            Chunk &chunk = m_Chunks[m_CurrentChunkIndex];

            const Count aligned = memory::Align( chunk.Offset, alignment );
            if ( aligned + size <= chunk.Size )
            {
                Byte *result = chunk.Data + aligned;
                chunk.Offset = aligned + size;
                return result;
            }

            // Move to the next existing chunk (carried over from a previous frame after Reset).
            const Index nextIndex = m_CurrentChunkIndex + 1;
            if ( nextIndex < m_Chunks.GetItemCount() && m_Chunks[nextIndex].Size >= size )
            {
                m_CurrentChunkIndex = nextIndex;
                continue;
            }

            // No existing chunk fits — allocate a new one large enough.
            AddChunk( size );
        }
    }

    void FrameArena::Reset() noexcept
    {
        for ( Chunk &chunk : m_Chunks )
            chunk.Offset = 0;

        m_CurrentChunkIndex = m_Chunks.IsEmpty() ? s_InvalidIndex : 0;
    }

    void FrameArena::ReleaseChunks() noexcept
    {
        for ( Chunk &chunk : m_Chunks )
        {
            if ( chunk.Data != nullptr )
                _aligned_free( chunk.Data );
        }
        m_Chunks.Clear();
        m_CurrentChunkIndex = s_InvalidIndex;
    }

    Count FrameArena::GetCommittedSize() const noexcept
    {
        Count total = 0;
        for ( const Chunk &chunk : m_Chunks )
            total += chunk.Size;
        return total;
    }

    Count FrameArena::GetUsedSize() const noexcept
    {
        Count total = 0;
        for ( const Chunk &chunk : m_Chunks )
            total += chunk.Offset;
        return total;
    }

    void FrameArena::AddChunk( const Count minSize )
    {
        // Round to a multiple of the max alignment to keep chunks aligned for any reasonable type.
        constexpr Count s_MaxArenaAlignment = 64;

        const Count size =
            memory::Align( minSize > m_InitialChunkSize ? minSize : m_InitialChunkSize, s_MaxArenaAlignment );

        Byte *data = static_cast< Byte * >( _aligned_malloc( size, s_MaxArenaAlignment ) );
        SM_ASSERT( data != nullptr );

        Chunk chunk{};
        chunk.Data = data;
        chunk.Size = size;
        chunk.Offset = 0;
        m_Chunks.PushBack( chunk );

        m_CurrentChunkIndex = m_Chunks.GetItemCount() - 1;
    }
}
