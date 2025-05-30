/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "system_allocator.h"

#include "smile/common/memory/in_place.h"

namespace smile::memory
{
    SystemAllocator &SystemAllocator::GetInstance()
    {
        static SystemAllocator *pInstance{ nullptr };
        static std::aligned_storage_t< sizeof( SystemAllocator ), std::alignment_of_v< SystemAllocator > > byteArray;

        if ( !pInstance )
        {
            pInstance = new ( &byteArray, g_pInPlace ) SystemAllocator;
        }

        return *pInstance;
    }

    void *SystemAllocator::CreateByteArray( const Uint32 size )
    {
        void *pByteArray = std::malloc( size );

        return pByteArray;
    }

    void SystemAllocator::DestroyByteArray( void *pByteArray )
    {
        std::free( reinterpret_cast< Byte * >( pByteArray ) );
    }

    void *SystemAllocator::InternalCreateByteArray( const Uint32 size, const void *pExtraData )
    {
        return CreateByteArray( size );
    }

    void SystemAllocator::InternalDestroyByteArray( void *pByteArray, const Uint32 size )
    {
        DestroyByteArray( pByteArray );
    }
}