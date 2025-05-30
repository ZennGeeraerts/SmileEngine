/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::memory
{
    constexpr Uint32 g_AlignedSize = 8; // x64
    constexpr Uint32 g_DefaultAllocatorAlignedSize = 8;

    constexpr Uint32 GetAlignedSize( const Uint32 size )
    {
        return ( size + g_AlignedSize - 1 ) & ~( g_AlignedSize - 1 );
    }

    Uint32 GetAllocatedSize( const Uint32 size, const bool addHeaderSize = true );

#if SM_C_DEBUG
    void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Uint32 size );
#else
    inline void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Uint32 size )
    {
        std::memmove( pDestByteArray, pSrcByteArray, size );
    }
#endif

    template < class Type >
    struct IsRawTypeTrait
    {
        static const bool value = std::is_pointer< Type >::value;
    };

    template < typename Type >
    constexpr bool IsRawType()
    {
        return IsRawTypeTrait< Type >::value;
    }

    template < typename ItemType >
    inline void MoveArrayItems( ItemType *pItemArray,
        const int itemCount,
        const ItemType *pOtherItemArray,
        typename std::enable_if< IsRawTypeTrait< ItemType >::value >::type * = nullptr )
    {
        MoveByteArray( pItemArray, pOtherItemArray, sizeof( *pItemArray ) * itemCount );
    }

    template < typename ItemType >
    inline void CopyArrayItems( ItemType *pItemArray,
        const int itemCount,
        const ItemType *pOtherItemArray,
        typename std::enable_if< !IsRawTypeTrait< ItemType >::value >::type * = nullptr )
    {
        SM_ASSERT( pItemArray < pOtherItemArray || pOtherItemArray + itemCount <= pItemArray );

        for ( int i = 0; i < itemCount; ++i )
        {
            pItemArray[i] = pOtherItemArray[i];
        }
    }

    template < typename ItemType >
    inline void CopyArrayItems( ItemType *pItemArray,
        const int itemCount,
        const ItemType *pOtherItemArray,
        typename std::enable_if< IsRawTypeTrait< ItemType >::value >::type * = nullptr )
    {
        MoveByteArray( pItemArray, pOtherItemArray, sizeof( *pItemArray ) * itemCount );
    }
}