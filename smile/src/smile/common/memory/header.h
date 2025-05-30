/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "memory.h"

namespace smile::memory
{
    class Header final
    {
      public:
        Header() = delete;
        Header( const Header & ) = delete;
        Header( Header && ) = delete;
        ~Header() = delete;

        Header &operator=( const Header & ) = delete;
        Header &operator=( Header && ) = delete;

        Uint32 GetSize() const
        {
            return Size;
        }

        void SetIsArray( const bool isArray )
        {
            IsArray = isArray;
        }

        bool GetIsArray() const
        {
            return static_cast< bool >( IsArray );
        }

        bool IsValid() const
        {
            return Signature == s_ValidSignature;
        }

        bool GetIsUsed() const
        {
            return IsUsed;
        }

        bool IsDestructable() const
        {
            return IsValid() && GetIsUsed();
        }

        void *GetByteArray()
        {
            return this + 1;
        }

        void SetAllocated( const Uint32 size, const Uint32 allocatorIndex );
        void SetDeallocated();

        const void *GetByteArray() const
        {
            return this + 1;
        }

        static inline Header *GetFromByteArray( void *pByteArray )
        {
            return reinterpret_cast< Header * >( pByteArray ) - 1;
        }

        constexpr static int s_InvalidSignature = 0;
        constexpr static int s_ValidSignature = 0x23AB7A;

        Uint32 Size : 30, IsUsed : 1, IsArray : 1;
        Uint32 Signature : 22, AllocatorIndex : 10;

        static Uint32 s_NextAllocatorIndex;
    };

    static_assert( ( sizeof( Header ) % g_AlignedSize ) == 0 );
    constexpr static int s_HeaderSize = sizeof( Header );
}