/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "header.h"

namespace smile::memory
{
    Uint32 Header::s_NextAllocatorIndex = 0;

    void Header::SetAllocated( const Uint32 size, const Uint32 allocatorIndex )
    {
        SM_ASSERT( size < ( 1 << 28 ) );
        SM_ASSERT( size >= 0 );
        SM_ASSERT( allocatorIndex >= 0 && allocatorIndex < 1024 );
        SM_ASSERT( !IsValid() );

        Size = size;
        IsUsed = 1;
        IsArray = false;
        AllocatorIndex = allocatorIndex;
        Signature = s_ValidSignature;
    }

    void Header::SetDeallocated()
    {
        SM_ASSERT( IsValid() );

#if SM_C_DEBUG
        Size = 0;
        IsUsed = 0;
#endif

        Signature = s_InvalidSignature;
    }
}

static_assert( sizeof( smile::memory::Header ) == smile::memory::g_DefaultAllocatorAlignedSize );