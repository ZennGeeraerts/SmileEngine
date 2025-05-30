/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/memory/allocator/system_allocator.h"
#include "smile/common/memory/memory.h"

#include <catch/catch.hpp>

namespace smile::memory
{
    class LocalMemoryStandardAllocator final : public Allocator
    {
      public:
        LocalMemoryStandardAllocator() = default;
        LocalMemoryStandardAllocator( const LocalMemoryStandardAllocator & ) = delete;
        LocalMemoryStandardAllocator( LocalMemoryStandardAllocator && ) = delete;
        ~LocalMemoryStandardAllocator() override = default;
        LocalMemoryStandardAllocator &operator=( const LocalMemoryStandardAllocator & ) = delete;
        LocalMemoryStandardAllocator &operator=( LocalMemoryStandardAllocator && ) = delete;

        virtual bool CanAllocateByteArray( const Uint32 size ) const override
        {
            return true;
        }

      protected:
        void *InternalCreateByteArray( const Uint32 size, const void *pExtraData ) override
        {
            return SystemAllocator::CreateByteArray( size );
        }

        void InternalDestroyByteArray( void *pByteArray, const Uint32 size ) override
        {
            SystemAllocator::DestroyByteArray( pByteArray );
        }
    };

    TEST_CASE( "memory::Allocator", "[memory]" )
    {
        SECTION( "Constructors" )
        {
            LocalMemoryStandardAllocator defaultConstructedAllocator;

            REQUIRE( &defaultConstructedAllocator ==
                     &Allocator::GetAllocatorByIndex( defaultConstructedAllocator.GetAllocatorIndex() ) );
            REQUIRE( 0 == defaultConstructedAllocator.GetAllocatedSize() );
        }

#if SM_C_DEBUG
        SECTION( "Create / Destroy ByteArray" )
        {
            int *pIntArray{ nullptr };
            LocalMemoryStandardAllocator allocator;

            REQUIRE( pIntArray == nullptr );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( 0 == allocator.GetAllocatedSize() );

            pIntArray = reinterpret_cast< int * >( allocator.CreateByteArray( 10 ) );

            REQUIRE( pIntArray != nullptr );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( GetAllocatedSize( 10 ) == allocator.GetAllocatedSize() );

            allocator.DestroyByteArray( pIntArray );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( 0 == allocator.GetAllocatedSize() );
        }

        SECTION( "Create / Destroy AlignedByteArray" )
        {
            int *pIntArray{ nullptr };
            LocalMemoryStandardAllocator allocator;

            REQUIRE( pIntArray == nullptr );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( 0 == allocator.GetAllocatedSize() );

            pIntArray = reinterpret_cast< int * >( allocator.CreateAlignedByteArray( 10, 4 ) );

            REQUIRE( pIntArray != nullptr );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( GetAllocatedSize( 10 + 4 ) == allocator.GetAllocatedSize() );

            allocator.DestroyAlignedByteArray( pIntArray );

            REQUIRE( &allocator == &Allocator::GetAllocatorByIndex( allocator.GetAllocatorIndex() ) );
            REQUIRE( 0 == allocator.GetAllocatedSize() );
        }
#endif
    }
}