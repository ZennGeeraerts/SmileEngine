/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "memory.h"

#include "header.h"
#include "allocator/system_allocator.h"

namespace smile::memory
{
    Allocator &GetAllocator()
    {
        static memory::Allocator *pAllocator{ nullptr };

        if ( !pAllocator )
        {
            pAllocator = &SystemAllocator::GetInstance();
        }

        return *pAllocator;
    }

    Count GetAllocatedSize( const Count size, const bool addHeaderSize )
    {
        return ( addHeaderSize ? s_HeaderSize : 0 ) + GetAlignedSize( size );
    }

    Header *GetHeader( const void *pObject )
    {
        const Header *pHeader = reinterpret_cast< const Header * >( pObject ) - 1;

        if ( !pHeader->IsValid() )
            pHeader = nullptr;

        return const_cast< Header * >( pHeader );
    }

    bool IsAllocatedObject( const void *pObject )
    {
        auto pHeader = GetHeader( pObject );

        return pHeader && !pHeader->IsArray && pHeader->IsUsed;
    }

#if SM_C_DEBUG
    void SetByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size )
    {
        if ( size > 0 )
        {
            SM_ASSERT( ( reinterpret_cast< const Byte * >( pDestByteArray ) + size ) <=
                           reinterpret_cast< const Byte * >( pSrcByteArray ) ||
                       ( reinterpret_cast< const Byte * >( pSrcByteArray ) + size ) <=
                           reinterpret_cast< const Byte * >( pDestByteArray ) );

            std::memcpy( pDestByteArray, pSrcByteArray, size );
        }
    }

    void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size )
    {
        if ( size > 0 )
        {
            std::memmove( pDestByteArray, pSrcByteArray, size );
        }
    }

    void FillByteArray( void *pDestByteArray, const Byte value, const Count size )
    {
        if ( size > 0 )
        {
            SM_ASSERT( pDestByteArray );
            std::memset( pDestByteArray, value, static_cast< std::size_t >( size ) );
        }
    }
#endif

    bool IsAllocatedByteArray( const void *pByteArray )
    {
        Header *pHeader = GetHeader( pByteArray );

        return pHeader && pHeader->IsUsed;
    }

    void *AllocateByteArray( const Count size )
    {
        void *pByteArray = GetAllocator().CreateByteArray( size );

        SM_ASSERT( pByteArray );

        return pByteArray;
    }

    void DeallocateByteArray( void *pByteArray )
    {
        Header *pHeader = GetHeader( pByteArray );

        if ( !pHeader )
        {
            SystemAllocator::DestroyByteArray( pByteArray );
        }
        else
        {
            pHeader->GetAllocator().DestroyByteArray( pByteArray );
        }
    }

    Count GetSize( void *pData )
    {
        return GetHeader( pData )->Size;
    }
}