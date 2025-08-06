/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/dictionary.h"
#include "smile/common/primitive/text/string.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "Dictionary" )
    {
        SECTION( "Add / GetItemAtKey" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            REQUIRE( dict.GetItemAtKey( "Test" ) == 1 );
            REQUIRE( dict.GetItemAtKey( "Test2" ) == 2 );

            REQUIRE( dict["Test"] == 1 );
            REQUIRE( dict["Test2"] == 2 );
        }

        SECTION( "GetKeys" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            const auto &keys = dict.GetKeys();

            REQUIRE( array::HasItem( keys, "Test" ) );
            REQUIRE( array::HasItem( keys, "Test2" ) );
            REQUIRE_FALSE( array::HasItem( keys, "Test3" ) );
        }

        SECTION( "GetItems" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            const auto &items = dict.GetItems();

            REQUIRE( array::HasItem( items, 1 ) );
            REQUIRE( array::HasItem( items, 2 ) );
            REQUIRE_FALSE( array::HasItem( items, 3 ) );
        }

        SECTION( "HasItemAtKey" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            REQUIRE( dict.HasItemAtKey( "Test" ) );
            REQUIRE( dict.HasItemAtKey( "Test2" ) );
            REQUIRE_FALSE( dict.HasItemAtKey( "Test3" ) );
        }

        SECTION( "GetItemCount" )
        {
            Dictionary< primitive::String, int > dict{};
            REQUIRE( dict.GetItemCount() == 0 );

            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            REQUIRE( dict.GetItemCount() == 2 );
        }

        SECTION( "FindItem" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            auto it = dict.FindItem( "Test" );
            REQUIRE( it.IsValid() );
            REQUIRE( ( *it ).Key == "Test" );
            REQUIRE( ( *it ).Item == 1 );

            it = dict.FindItem( "Test2" );
            REQUIRE( it.IsValid() );
            REQUIRE( ( *it ).Key == "Test2" );
            REQUIRE( ( *it ).Item == 2 );

            it = dict.FindItem( "Test3" );
            REQUIRE_FALSE( it.IsValid() );
        }

        SECTION( "EraseAtKey" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            dict.EraseAtKey( "Test" );

            REQUIRE_FALSE( dict.FindItem( "Test" ).IsValid() );
            REQUIRE( dict.FindItem( "Test2" ).IsValid() );
            REQUIRE( dict.GetItemCount() == 1 );
        }

        SECTION( "Erase" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            auto it = dict.FindItem( "Test" );
            dict.Erase( it );

            REQUIRE_FALSE( dict.FindItem( "Test" ).IsValid() );
            REQUIRE( dict.FindItem( "Test2" ).IsValid() );
            REQUIRE( dict.GetItemCount() == 1 );
        }

        SECTION( "Clear" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            dict.Clear();

            REQUIRE( dict.IsEmpty() );
        }

        SECTION( "IsEmpty" )
        {
            Dictionary< primitive::String, int > dict{};

            REQUIRE( dict.IsEmpty() );

            dict.Add( "Test", 1 );

            REQUIRE_FALSE( dict.IsEmpty() );
        }

        SECTION( "begin" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            auto iteratorBegin = dict.begin();
            REQUIRE( iteratorBegin.IsValid() );
            REQUIRE( iteratorBegin.GetItem() == 1 );
            REQUIRE( iteratorBegin.GetKey() == "Test" );
        }

        SECTION( "end" )
        {
            Dictionary< primitive::String, int > dict{};
            dict.Add( "Test", 1 );
            dict.Add( "Test2", 2 );

            auto iteratorEnd = dict.end();
            REQUIRE_FALSE( iteratorEnd.IsValid() );
        }
    }
}