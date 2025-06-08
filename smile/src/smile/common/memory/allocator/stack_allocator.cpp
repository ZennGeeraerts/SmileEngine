/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "stack_allocator.h"

namespace smile::memory
{
    StackAllocator::~StackAllocator()
    {
        SM_ASSERT( ( m_InternalAllocatedSize == 0 ) || ( m_AllocationCount == 0 ) );
    }

    bool StackAllocator::CanAllocateByteArray( const Uint32 size ) const
    {
        return size <= m_AvailableSize;
    }

    void StackAllocator::Initialize( void *pAllocatedByteArray, const Uint32 allocatedSize, bool ownsMemory )
    {
        SM_ASSERT( m_InternalAllocatedSize == 0 );

        AreaAllocator::Initialize( pAllocatedByteArray, allocatedSize, ownsMemory );

        m_AvailableSize = allocatedSize;
        m_pCurrentByteArray = reinterpret_cast< Byte * >( pAllocatedByteArray );
        m_InternalAllocatedSize = 0;
        m_AllocationCount = 0;
    }

    void StackAllocator::Reset()
    {
        m_AvailableSize = m_AllocatedSize;
        m_pCurrentByteArray = reinterpret_cast< Byte * >( m_pAllocatedByteArray );
        m_InternalAllocatedSize = 0;
        m_AllocationCount = 0;
#if SM_C_DEBUG
        m_AllocatedSize = 0;
#endif
    }

    void *StackAllocator::InternalCreateByteArray( const Uint32 size, const void * )
    {
        SM_ASSERT( size == memory::GetAlignedSize( size ) );

        if ( size > m_AvailableSize )
            return nullptr;

        void *pResult = m_pCurrentByteArray;
        m_pCurrentByteArray += size;
        m_InternalAllocatedSize += size;
        m_AvailableSize -= size;
        ++m_AllocationCount;

        return pResult;
    }

    void StackAllocator::InternalDestroyByteArray( void *, const Uint32 size )
    {
        SM_ASSERT( size == memory::GetAlignedSize( size ) );

        m_InternalAllocatedSize -= size;
        --m_AllocationCount;

        if ( m_InternalAllocatedSize == 0 )
        {
            m_AvailableSize = m_AllocatedSize;
            m_pCurrentByteArray = reinterpret_cast< Byte * >( m_pAllocatedByteArray );
        }
    }
}