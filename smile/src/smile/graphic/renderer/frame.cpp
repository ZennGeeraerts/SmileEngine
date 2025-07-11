/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "frame.h"

#include "smile/common/foundation/range_iterator.h"
#include "smile/common/memory/allocator/stack_allocator.h"
#include "smile/common/primitive/collection/vector.h"

#include <mutex>

namespace smile::graphic
{
    std::mutex g_Mutex;
    Index g_CurrentFrame{ 0 };
    Index g_LastFrameReleased{ 0 };

    Index g_CurrentFrameFirstAllocatorIndex = s_InvalidIndex;
    Index g_CurrentFrameLastAllocatorIndex = s_InvalidIndex;

    struct FrameAllocator final
    {
        std::optional< Index > FrameIndex;
        memory::StackAllocator Allocator;
    };

    primitive::Vector< Scope< FrameAllocator > > g_pFrameAllocators;

    static constexpr Count s_FrameAllocatorSize = 64 * 1024;

    void AddFrameAllocator( const Index index )
    {
        auto pAllocator = CreateScope< FrameAllocator >();

        pAllocator->Allocator.SetUseHeader( false );
        pAllocator->Allocator.Initialize(
            memory::AllocateByteArray( s_FrameAllocatorSize ), s_FrameAllocatorSize, true );

        g_pFrameAllocators.Insert( std::move( pAllocator ), index );
    }

    Index AssignFrameData()
    {
        std::lock_guard lock{ g_Mutex };

        ++g_CurrentFrame;
        if ( g_CurrentFrame == 0 )
            ++g_CurrentFrame;

        if ( g_pFrameAllocators.IsEmpty() )
        {
            AddFrameAllocator( 0 );
        }

        auto nextFreeAllocatorIndex = ( g_CurrentFrameLastAllocatorIndex + 1 ) % g_pFrameAllocators.GetItemCount();
        if ( g_pFrameAllocators[nextFreeAllocatorIndex]->FrameIndex.has_value() )
        {
            AddFrameAllocator( nextFreeAllocatorIndex );
        }

        g_CurrentFrameFirstAllocatorIndex = g_CurrentFrameLastAllocatorIndex = nextFreeAllocatorIndex;
        g_pFrameAllocators[nextFreeAllocatorIndex]->FrameIndex = g_CurrentFrame;

        SM_ASSERT( g_pFrameAllocators[nextFreeAllocatorIndex]->Allocator.GetAllocationCount() == 0 );

        return g_CurrentFrame;
    }

    Byte *AllocateFrameData( const Count size )
    {
        std::lock_guard lock{ g_Mutex };

        SM_ASSERT( size == s_FrameAllocatorSize );

        for ( auto index :
            foundation::GetRangeIterator( g_CurrentFrameFirstAllocatorIndex, g_CurrentFrameLastAllocatorIndex + 1 ) )
        {
            auto loopingIndex = index % g_pFrameAllocators.GetItemCount();

            SM_ASSERT( g_pFrameAllocators[loopingIndex]->FrameIndex == g_CurrentFrame );

            auto &currentAllocator = g_pFrameAllocators[loopingIndex]->Allocator;
            if ( currentAllocator.CanAllocateByteArray( size ) )
            {
                return reinterpret_cast< Byte * >( currentAllocator.CreateByteArray( size ) );
            }
        }

        // No memory found, get the next frame allocator

        auto nextIndex = ( g_CurrentFrameLastAllocatorIndex + 1 ) % g_pFrameAllocators.GetItemCount();

        if ( !g_pFrameAllocators.IsValidIndex( nextIndex ) || g_pFrameAllocators[nextIndex]->FrameIndex.has_value() )
        {
            AddFrameAllocator( nextIndex );
        }

        ++g_CurrentFrameLastAllocatorIndex;
        g_pFrameAllocators[nextIndex]->FrameIndex = g_CurrentFrame;

        SM_ASSERT( g_pFrameAllocators[nextIndex]->Allocator.GetAllocationCount() );

        return reinterpret_cast< Byte * >( g_pFrameAllocators[nextIndex]->Allocator.CreateByteArray( size ) );
    }

    void ReleaseFrameData( const Index frameIndex )
    {
        if ( frameIndex == 0 )
            return;

        std::lock_guard lock{ g_Mutex };

        Index nextFrameIndex = g_LastFrameReleased + 1;
        if ( nextFrameIndex == 0 )
            ++nextFrameIndex;

        SM_ASSERT( nextFrameIndex == frameIndex );

        g_LastFrameReleased = frameIndex;

        for ( auto &pAllocator : g_pFrameAllocators )
        {
            if ( pAllocator->FrameIndex == frameIndex )
            {
                pAllocator->FrameIndex.reset();
                pAllocator->Allocator.Reset();
            }
        }
    }
}