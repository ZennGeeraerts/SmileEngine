/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/stream/memory_binary_stream.h"

#include <catch/catch.hpp>

namespace smile
{
    TEST_CASE( "MemoryBinaryStream", "[stream]" )
    {
        SECTION( "OpenInput" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenInput();

            REQUIRE( stream.IsOpen() );
            REQUIRE( stream.IsInput() );
            REQUIRE( !stream.IsOutput() );

            stream.Close();
        }

        SECTION( "OpenOutput" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenOutput( { stream::OpeningMode::Append } );

            REQUIRE( stream.IsOpen() );
            REQUIRE( !stream.IsInput() );
            REQUIRE( stream.IsOutput() );

            stream.Close();
        }

        SECTION( "OpenInputOutput" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenInputOutput( { stream::OpeningMode::Append } );

            REQUIRE( stream.IsOpen() );
            REQUIRE( stream.IsInput() );
            REQUIRE( stream.IsOutput() );

            stream.Close();
        }

        SECTION( "WriteByteArray" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenOutput( { stream::OpeningMode::Append } );

            std::vector< Uint32 > dummyData{ 1, 2, 4, 8, 16 };
            stream.WriteByteArray( dummyData.data(), static_cast< Uint32 >( dummyData.size() ) * sizeof( Uint32 ) );

            stream.Close();

            const auto &byteArray = stream.GetByteArray();

            const auto count = byteArray.size() / sizeof( Uint32 );
            std::vector< Uint32 > result{ reinterpret_cast< const Uint32 * >( byteArray.data() ),
                reinterpret_cast< const Uint32 * >( byteArray.data() ) + count };

            REQUIRE( dummyData.size() == result.size() );

            for ( Uint32 i{}; i < dummyData.size(); ++i )
            {
                REQUIRE( dummyData[i] == result[i] );
            }
        }

        SECTION( "ReadByteArray" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenInputOutput( { stream::OpeningMode::Append } );

            std::vector< Uint32 > dummyData{ 1, 2, 4, 8, 16 };
            stream.WriteByteArray( dummyData.data(), static_cast< Uint32 >( dummyData.size() ) * sizeof( Uint32 ) );

            stream.Close();

            stream.OpenInput();

            std::vector< Uint32 > result( dummyData.size() );
            stream.ReadByteArray( result.data(), result.size() * sizeof( Uint32 ) );

            stream.Close();

            REQUIRE( dummyData.size() == result.size() );

            for ( Uint32 i{}; i < dummyData.size(); ++i )
            {
                REQUIRE( dummyData[i] == result[i] );
            }
        }
    }
}