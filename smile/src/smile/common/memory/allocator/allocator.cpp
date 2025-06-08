/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "allocator.h"

#include "smile/common/memory/memory.h"
#include "smile/common/memory/header.h"

namespace smile
{
    std::atomic< UintPointer > g_FirstFreeAllocatorIndex =
        static_cast< UintPointer >( std::numeric_limits< Uint32 >::max() );

    std::atomic< Uint32 > g_AllocatorCount = 0;
}

namespace smile::memory
{
    Allocator *Allocator::s_pAllocators[s_MaxAllocatorCount];

    Allocator::Allocator()
        : m_AllocatorIndex{ std::numeric_limits< Uint32 >::max() },
          m_UseHeader{ true },
          m_BreaksOnOutOfMemory{ true }
#ifdef SM_C_DEBUG
          ,
          m_AllocatedSize{ 0 }
#endif
    {
        AddAllocator( *this );
    }

    Allocator::~Allocator()
    {
        RemoveAllocator( *this );
    }

    void Allocator::DestroyByteArray( void *pByteArray )
    {
        if ( !pByteArray )
            return;

        if ( m_UseHeader )
        {
            Header *pHeader = Header::GetFromByteArray( pByteArray );
            SM_ASSERT( pHeader->IsDestructable() );

            Uint32 allocatedSize = memory::GetAllocatedSize( pHeader->Size, true );
#if SM_C_DEBUG
            m_AllocatedSize -= allocatedSize;
            SM_ASSERT( m_AllocatedSize >= 0 );
#endif

            pHeader->SetDeallocated();

            InternalDestroyByteArray( pHeader, allocatedSize );
        }
        else
        {
            InternalDestroyByteArray( pByteArray, 0 );
        }
    }

    void Allocator::DestroyAlignedByteArray( void *pByteArray )
    {
        SM_ASSERT( pByteArray != nullptr );

        auto alignedOffset = reinterpret_cast< Uint32 * >( pByteArray )[-1];
        pByteArray = reinterpret_cast< Byte * >( pByteArray ) - alignedOffset;
        DestroyByteArray( pByteArray );
    }

    void Allocator::AddAllocator( Allocator &allocator )
    {
        auto pFreeAllocatorIndices = reinterpret_cast< UintPointer * >( s_pAllocators );

        if ( g_AllocatorCount == 0 && g_FirstFreeAllocatorIndex == std::numeric_limits< Uint32 >::max() )
        {
            for ( auto allocatorIndex = 0; allocatorIndex < s_MaxAllocatorCount - 1; ++allocatorIndex )
            {
                pFreeAllocatorIndices[allocatorIndex] = allocatorIndex + 1;
            }

            pFreeAllocatorIndices[s_MaxAllocatorCount - 1] = UintPointer( std::numeric_limits< Uint32 >::max() );
            g_FirstFreeAllocatorIndex = 0;
        }

        SM_ASSERT( Uint32( g_FirstFreeAllocatorIndex ) != std::numeric_limits< Uint32 >::max() );

        auto currentIndex = g_FirstFreeAllocatorIndex.load( std::memory_order_relaxed );
        auto nextFreeAllocatorIndex = pFreeAllocatorIndices[currentIndex];

        while ( !g_FirstFreeAllocatorIndex.compare_exchange_weak(
            currentIndex, nextFreeAllocatorIndex, std::memory_order_release, std::memory_order_relaxed ) )
        {
            nextFreeAllocatorIndex = pFreeAllocatorIndices[currentIndex];
        }

        allocator.m_AllocatorIndex = Uint32( currentIndex );
        ++g_AllocatorCount;

        s_pAllocators[allocator.m_AllocatorIndex] = &allocator;
    }

    void Allocator::RemoveAllocator( Allocator &allocator )
    {
        SM_ASSERT( s_pAllocators[allocator.m_AllocatorIndex] == &allocator );

        auto allocatorNextIndexSlot = &reinterpret_cast< UintPointer * >( s_pAllocators )[allocator.m_AllocatorIndex];

        *allocatorNextIndexSlot = g_FirstFreeAllocatorIndex.load( std::memory_order_relaxed );
        while ( !g_FirstFreeAllocatorIndex.compare_exchange_weak( *allocatorNextIndexSlot,
            allocator.m_AllocatorIndex,
            std::memory_order_release,
            std::memory_order_relaxed ) )
        {
        }

        allocator.m_AllocatorIndex = std::numeric_limits< Uint32 >::max();

        --g_AllocatorCount;
    }

    Uint32 Allocator::GetAllocatorCount()
    {
        return g_AllocatorCount;
    }

    void *Allocator::CreateByteArray( const Uint32 size, const void *pExtraData )
    {
        if ( size == 0 )
            return nullptr;

        auto allocatedSize = memory::GetAllocatedSize( size, m_UseHeader );
        auto pHeader = reinterpret_cast< Header * >( InternalCreateByteArray( allocatedSize, pExtraData ) );

        if ( !pHeader )
        {
            SM_ASSERT_MSG( m_BreaksOnOutOfMemory, "ERROR: Not enough memory in allocator {}", m_AllocatorIndex );

            return nullptr;
        }

#if SM_C_DEBUG
        m_AllocatedSize += allocatedSize;
#endif

        if ( m_UseHeader )
        {
            pHeader->SetAllocated( size, m_AllocatorIndex );
            return pHeader->GetByteArray();
        }
        else
        {
            return pHeader;
        }
    }

    void *Allocator::CreateAlignedByteArray( const Uint32 size, const Uint32 alignedSize, const void *pExtraData )
    {
        SM_ASSERT( size > 0 );
        SM_ASSERT( alignedSize >= 4 );
        SM_ASSERT( ( alignedSize % 4 ) == 0 );

        void *pByteArray = CreateByteArray( size + alignedSize, pExtraData );
        auto alignedOffset = static_cast< Uint32 >( alignedSize - ( UintPointer( pByteArray ) % alignedSize ) );
        pByteArray = reinterpret_cast< Byte * >( pByteArray ) + alignedOffset;
        reinterpret_cast< Uint32 * >( pByteArray )[-1] = alignedOffset;

        return pByteArray;
    }
}