/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "memory.h"

#include "header.h"

namespace smile::memory
{
    Uint32 GetAllocatedSize( const Uint32 size, const bool addHeaderSize )
    {
        return ( addHeaderSize ? s_HeaderSize : 0 ) + GetAlignedSize( size );
    }

#if SM_C_DEBUG
    void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Uint32 size )
    {
        if ( size > 0 )
        {
            std::memmove( pDestByteArray, pSrcByteArray, size );
        }
    }
#endif
}