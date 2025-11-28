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

            primitive::Vector< Uint32 > dummyData{ 1, 2, 4, 8, 16 };
            stream.WriteByteArray( dummyData.GetData(), dummyData.GetItemCount() * sizeof( Uint32 ) );

            stream.Close();

            const auto &byteArray = stream.GetByteArray();

            const Count count = byteArray.GetItemCount() / sizeof( Count );
            primitive::Vector< Uint32 > result{ reinterpret_cast< const Uint32 * >( byteArray.GetData() ), count };

            REQUIRE( dummyData.GetItemCount() == result.GetItemCount() );

            for ( Index i{}; i < dummyData.GetItemCount(); ++i )
            {
                REQUIRE( dummyData[i] == result[i] );
            }
        }

        SECTION( "ReadByteArray" )
        {
            stream::MemoryBinaryStream stream{};
            stream.OpenInputOutput( { stream::OpeningMode::Append } );

            primitive::Vector< Uint32 > dummyData{ 1, 2, 4, 8, 16 };
            stream.WriteByteArray( dummyData.GetData(), dummyData.GetItemCount() * sizeof( Uint32 ) );

            stream.Close();

            stream.OpenInput();

            primitive::Vector< Uint32 > result( dummyData.GetItemCount() );
            stream.ReadByteArray( result.GetData(), result.GetItemCount() * sizeof( Uint32 ) );

            stream.Close();

            REQUIRE( dummyData.GetItemCount() == result.GetItemCount() );

            for ( Index i{}; i < dummyData.GetItemCount(); ++i )
            {
                REQUIRE( dummyData[i] == result[i] );
            }
        }
    }
}