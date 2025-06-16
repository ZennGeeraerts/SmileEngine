/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "area_allocator.h"

#include "smile/common/memory/memory.h"

namespace smile::memory
{
    AreaAllocator::~AreaAllocator()
    {
        if ( m_OwnsMemory && m_pAllocatedByteArray && memory::IsAllocatedByteArray( m_pAllocatedByteArray ) )
        {
            memory::DeallocateByteArray( m_pAllocatedByteArray );
        }
    }

    void AreaAllocator::Initialize( void *pAllocatedByteArray, const Uint32 allocatedSize, bool ownsMemory )
    {
        SM_ASSERT( pAllocatedByteArray );
        SM_ASSERT( allocatedSize > 0 );
        SM_ASSERT( !HasAllocatedByteArray() );

        m_pAllocatedByteArray = pAllocatedByteArray;
        m_AllocatedSize = allocatedSize;
        m_OwnsMemory = ownsMemory;
    }
}