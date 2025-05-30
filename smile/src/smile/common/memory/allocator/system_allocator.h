/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "allocator.h"

namespace smile::memory
{
    class SystemAllocator final : public Allocator
    {
      public:
        SystemAllocator() = default;
        SystemAllocator( const SystemAllocator & ) = delete;
        SystemAllocator( SystemAllocator && ) = delete;

        SystemAllocator &operator=( const SystemAllocator & ) = delete;
        SystemAllocator &operator=( SystemAllocator && ) = delete;

        bool CanAllocateByteArray( const Uint32 size ) const override
        {
            return true;
        }

        static SystemAllocator &GetInstance();

        static void *CreateByteArray( const Uint32 size );
        static void DestroyByteArray( void *pByteArray );

      protected:
        void *InternalCreateByteArray( const Uint32 size, const void *pExtraData ) override;
        void InternalDestroyByteArray( void *pByteArray, const Uint32 size ) override;
    };
}