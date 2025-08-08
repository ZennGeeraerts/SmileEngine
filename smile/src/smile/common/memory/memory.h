/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::memory
{
    class Header;
    class Allocator;
}

#include "in_place.h"

#define INTERNAL_MEMORY_INCLUDED_GUARD

namespace smile::memory
{
    constexpr Uint32 g_AlignedSize = 8; // x64
    constexpr Uint32 g_DefaultAllocatorAlignedSize = 8;

    constexpr Uint32 GetAlignedSize( const Uint32 size )
    {
        return ( size + g_AlignedSize - 1 ) & ~( g_AlignedSize - 1 );
    }

    Allocator &GetAllocator();
    Uint32 GetAllocatedSize( const Uint32 size, const bool addHeaderSize = true );
    Header *GetHeader( const void *pObject );

#if SM_C_DEBUG
    void SetByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size );
    void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size );
    void FillByteArray( void *pDestByteArray, const Byte value, const Count size );
#else
    inline void SetByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size )
    {
        std::memcpy( pDestByteArray, pSrcByteArray, size );
    }

    inline void MoveByteArray( void *pDestByteArray, const void *pSrcByteArray, const Count size )
    {
        std::memmove( pDestByteArray, pSrcByteArray, size );
    }

    inline void FillByteArray( void *pDestByteArray, const Byte value, const Count size )
    {
        std::memset( pDestByteArray, value, size );
    }
#endif

    bool IsAllocatedByteArray( const void *pByteArray );
    void *AllocateByteArray( const Uint32 size );
    void DeallocateByteArray( void *pByteArray );
    Uint32 GetSize( void *pData );

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
    inline void ConstructArrayItems( ItemType *pItems,
        const Count itemCount,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        for ( Index index = 0; index < itemCount; ++index )
        {
            ::new ( pItems + index, g_pInPlace ) ItemType;
        }
    }

    template < typename ItemType >
    inline void ConstructArrayItems( ItemType *,
        const Count,
        typename std::enable_if_t< IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
    }

    template < typename ItemType, class OtherItemType >
    inline void ConstructCopiedArrayItems( ItemType *pItems,
        const Count itemCount,
        const OtherItemType *pOtherItems,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value || !std::is_same_v< ItemType, OtherItemType > >
            * = nullptr )
    {
        for ( Index index = 0; index < itemCount; ++index )
        {
            ::new ( pItems + index, memory::g_pInPlace ) ItemType{ pOtherItems[index] };
        }
    }

    template < typename ItemType, class OtherItemType >
    inline void ConstructCopiedArrayItems( ItemType *pItems,
        const Count itemCount,
        const OtherItemType *pOtherItems,
        typename std::enable_if_t< !(
            !IsRawTypeTrait< ItemType >::value || !std::is_same_v< ItemType, OtherItemType > ) > * = nullptr )
    {
        SetByteArray( pItems, pOtherItems, sizeof( *pItems ) * itemCount );
    }

    template < typename ItemType >
    inline void ConstructMoveArrayItems( ItemType *pItems,
        const Count itemCount,
        ItemType *pOtherItems,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        for ( Index index = 0; index < itemCount; ++index )
        {
            ::new ( pItems + index, memory::g_pInPlace ) ItemType{ std::move( pOtherItems[index] ) };
        }
    }

    template < typename ItemType >
    inline void ConstructMoveArrayItems( ItemType *pItems,
        const Count itemCount,
        ItemType *pOtherItems,
        typename std::enable_if_t< IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        SetByteArray( pItems, pOtherItems, sizeof( *pItems ) * itemCount );
    }

    template < typename ItemType >
    inline void MoveArrayItems( ItemType *pItems,
        const int itemCount,
        const ItemType *pOtherItems,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        SM_ASSERT( pItems < pOtherItems || pOtherItems + itemCount <= pItems );

        for ( auto index = 0; index < itemCount; ++index )
        {
            pItems[index] = std::move( pOtherItems[index] );
        }
    }

    template < typename ItemType >
    inline void MoveArrayItems( ItemType *pItems,
        const int itemCount,
        const ItemType *pOtherItems,
        typename std::enable_if_t< IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        MoveByteArray( pItems, pOtherItems, sizeof( *pItems ) * itemCount );
    }

    template < typename ItemType >
    inline void CopyArrayItems( ItemType *pItems,
        const int itemCount,
        const ItemType *pOtherItems,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        SM_ASSERT( pItems < pOtherItems || pOtherItems + itemCount <= pItems );

        for ( Index index = 0; index < itemCount; ++index )
        {
            pItems[index] = pOtherItems[index];
        }
    }

    template < typename ItemType >
    inline void CopyArrayItems( ItemType *pItems,
        const int itemCount,
        const ItemType *pOtherItemArray,
        typename std::enable_if_t< IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        MoveByteArray( pItems, pOtherItemArray, sizeof( *pItems ) * itemCount );
    }

    template < typename ItemType >
    inline void DestructArrayItems( ItemType *pItems,
        const Count itemCount,
        typename std::enable_if_t< !IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
        for ( Index index = 0; index < itemCount; ++index )
        {
            pItems[index].~ItemType();
        }
    }

    template < typename ItemType >
    inline void DestructArrayItems( ItemType *,
        const Count,
        typename std::enable_if_t< IsRawTypeTrait< ItemType >::value > * = nullptr )
    {
    }
}

#include "header.h"

namespace smile::memory
{
    inline bool CompareByteArrays( const void *pFirstByteArray, const void *pSecondByteArray, const Count size )
    {
        return std::memcmp( pFirstByteArray, pSecondByteArray, size ) == 0;
    }

    template < typename ItemType >
    void AllocateUninitializedArray( ItemType *&pItems, const Count itemCount )
    {
        pItems = reinterpret_cast< ItemType * >( AllocateByteArray( itemCount * sizeof( ItemType ) ) );
        Header::GetFromByteArray( pItems )->SetIsArray( true );
    }
}