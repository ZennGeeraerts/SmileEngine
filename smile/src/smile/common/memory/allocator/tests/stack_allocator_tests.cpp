/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/memory/allocator/stack_allocator.h"
#include "smile/common/memory/header.h"

#include <catch/catch.hpp>

namespace smile::memory
{
    TEST_CASE( "StackAllocator", "[memory]" )
    {
        SECTION( "Constructors" )
        {
            StackAllocator defaultConstructed;

            REQUIRE( &defaultConstructed == &Allocator::GetAllocatorByIndex( defaultConstructed.GetAllocatorIndex() ) );
            REQUIRE_FALSE( defaultConstructed.HasAllocatedByteArray() );
        }

        SECTION( "Initialize" )
        {
            int intArray[128];
            StackAllocator allocator;

            allocator.Initialize( intArray, 128, false );

            REQUIRE( allocator.HasAllocatedByteArray() );
        }

        SECTION( "Check Overflow" )
        {
            StackAllocator *pAllocator = new StackAllocator;
            Byte *pByteArray = new Byte[1024];
            void *pAllocated = nullptr;

            pAllocator->Initialize( pByteArray, 1024, false );

            REQUIRE_ASSERT( pAllocator->CreateByteArray( 2048 ) );
            REQUIRE_NOASSERT( pAllocated = pAllocator->CreateByteArray( 512 ) );
            REQUIRE( pAllocated );

            pAllocator->DestroyByteArray( pAllocated );

            delete[] pByteArray;
            delete pAllocator;
        }

        SECTION( "Headers" )
        {
            Byte buffer[1024];
            void *pAllocated;

            {
                StackAllocator allocator;
                allocator.SetUseHeader( true );
                allocator.Initialize( buffer, 1024, false );
                REQUIRE_ASSERT( pAllocated = allocator.CreateByteArray( 1024 ) );
            }

            {
                StackAllocator allocator;
                allocator.SetUseHeader( true );
                allocator.Initialize( buffer, 1024, false );
                REQUIRE_NOASSERT( pAllocated = allocator.CreateByteArray( 512 ) );

                REQUIRE( buffer + s_HeaderSize == pAllocated );

                allocator.DestroyByteArray( pAllocated );
            }

            {
                StackAllocator allocator;
                allocator.SetUseHeader( false );
                allocator.Initialize( buffer, 1024, false );
                REQUIRE_NOASSERT( pAllocated = allocator.CreateByteArray( 1024 ) );

                REQUIRE( buffer == pAllocated );

                allocator.DestroyByteArray( pAllocated );
            }
        }

        SECTION( "Create / Destroy" )
        {
            Byte buffer[512];
            void *pAllocatedArrays[4];
            StackAllocator allocator;

            allocator.Initialize( buffer, 512, false );

            REQUIRE_NOASSERT( pAllocatedArrays[0] = allocator.CreateByteArray( 512 - s_HeaderSize ) );

            REQUIRE( 0 == allocator.GetAvailableSize() );

            allocator.DestroyByteArray( pAllocatedArrays[0] );

            REQUIRE( 512 == allocator.GetAvailableSize() );

            REQUIRE_NOASSERT( pAllocatedArrays[0] = allocator.CreateByteArray( 128 - s_HeaderSize ) );
            REQUIRE_NOASSERT( pAllocatedArrays[1] = allocator.CreateByteArray( 128 - s_HeaderSize ) );
            REQUIRE_NOASSERT( pAllocatedArrays[2] = allocator.CreateByteArray( 128 - s_HeaderSize ) );
            REQUIRE_NOASSERT( pAllocatedArrays[3] = allocator.CreateByteArray( 128 - s_HeaderSize ) );

            REQUIRE( 0 == allocator.GetAvailableSize() );

            allocator.DestroyByteArray( pAllocatedArrays[0] );
            allocator.DestroyByteArray( pAllocatedArrays[1] );
            allocator.DestroyByteArray( pAllocatedArrays[2] );
            allocator.DestroyByteArray( pAllocatedArrays[3] );

            REQUIRE( 512 == allocator.GetAvailableSize() );
        }
    }
}