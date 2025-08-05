/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/hash_table.h"
#include "smile/common/foundation/identifier.h"

#include <catch/catch.hpp>

namespace
{
    class DummyKey final
    {
      public:
        DummyKey( const char *text ) : Text{ text }
        {
        }

        bool operator==( const DummyKey &other ) const
        {
            return std::strcmp( Text, other.Text ) == 0;
        }

        bool operator!=( const DummyKey &other ) const
        {
            return !( *this == other );
        }

        const char *Text;
    };

    class DummyClass final
    {
      public:
        using KeyType = DummyKey;
        using ItemType = int;

        DummyClass() = delete;

        DummyClass( const int value, KeyType key ) : Value{ value }, Key{ std::move( key ) }
        {
        }

        DummyClass( const DummyClass & ) = delete;

        ~DummyClass() = default;

        const KeyType &GetKey() const
        {
            return Key;
        }

        const ItemType &GetItem() const
        {
            return Value;
        }

        ItemType &GetItem()
        {
            return Value;
        }

        DummyClass *GetNextNode() const
        {
            return pNextNode;
        }

        void SetNextNode( DummyClass *pNode )
        {
            pNextNode = pNode;
        }

        bool operator==( const DummyClass &other ) const
        {
            return Value == other.Value && Key == other.Key;
        }

        bool operator!=( const DummyClass &other ) const
        {
            return Value != other.Value || Key != other.Key;
        }

        ItemType Value;
        KeyType Key;
        DummyClass *pNextNode;
        smile::foundation::HashCode Hash{ 0 };
    };
}

namespace std
{
    template <>
    struct hash< DummyKey >
    {
        size_t operator()( const DummyKey &key ) const
        {
            return smile::foundation::identifier::GetHashCode( key.Text );
        }
    };
}

namespace
{
    class Fixture
    {
      public:
        Fixture() : FirstItem{ 166, "first_key" }, SecondItem{ 666, "second_key" }, ThirdItem{ 5150, "third_key" }
        {
            TestTable.Insert( &FirstItem );
            TestTable.Insert( &SecondItem );
            TestTable.Insert( &ThirdItem );
        }

        ~Fixture() = default;

        smile::primitive::HashTable< DummyClass > TestTable;
        DummyClass FirstItem, SecondItem, ThirdItem;
    };
}

namespace smile::primitive
{
    TEST_CASE_METHOD( Fixture, "Constructors", "[primitive]" )
    {
        DummyClass first{ 166, "first_key" }, second{ 1662, "test2" };
        HashTable< DummyClass > table;

        table.Insert( &first );
        table.Insert( &second );

        REQUIRE( table.GetItemAtKey( "first_key" ) == 166 );
        REQUIRE( table.GetItemAtKey( "test2" ) == 1662 );
    }

    TEST_CASE_METHOD( Fixture, "Position", "[primitive]" )
    {
        bool valueWasFound[3];

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            valueWasFound[index] = false;
        }

        auto positionCount = 0;

        for ( auto item : TestTable )
        {
            switch ( item.Value )
            {
                case 166:
                {
                    valueWasFound[0] = true;
                }
                break;

                case 666:
                {
                    valueWasFound[1] = true;
                }
                break;

                case 5150:
                {
                    valueWasFound[2] = true;
                }
                break;
            }

            ++positionCount;
        }

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            REQUIRE( valueWasFound[index] );
        }

        REQUIRE( 3 == positionCount );
    }

    TEST_CASE_METHOD( Fixture, "ConstPosition", "[primitive]" )
    {
        bool valueWasFound[3];

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            valueWasFound[index] = false;
        }

        auto positionCount = 0;

        const auto &constTestTable = TestTable;

        for ( auto item : constTestTable )
        {
            switch ( item.Value )
            {
                case 166:
                {
                    valueWasFound[0] = true;
                }
                break;

                case 666:
                {
                    valueWasFound[1] = true;
                }
                break;

                case 5150:
                {
                    valueWasFound[2] = true;
                }
                break;
            }

            ++positionCount;
        }

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            REQUIRE( valueWasFound[index] );
        }

        REQUIRE( 3 == positionCount );
    }

    TEST_CASE_METHOD( Fixture, "IsEmpty", "[primitive]" )
    {
        primitive::HashTable< DummyClass > emptyTable;

        REQUIRE( emptyTable.IsEmpty() );
        REQUIRE_FALSE( TestTable.IsEmpty() );

        TestTable.Clear();
        REQUIRE( TestTable.IsEmpty() );
    }

    TEST_CASE_METHOD( Fixture, "GetItemAtKey", "[primitive]" )
    {
        {
            REQUIRE( TestTable.GetItemAtKey( "first_key" ) == 166 );
            REQUIRE( TestTable.GetItemAtKey( "second_key" ) == 666 );
            REQUIRE( TestTable.GetItemAtKey( "third_key" ) == 5150 );
            REQUIRE_ASSERT( TestTable.GetItemAtKey( "unknown_key" ) );
        }

        {
            const HashTable< DummyClass > &constTestTable = TestTable;

            REQUIRE( constTestTable.GetItemAtKey( "first_key" ) == 166 );
            REQUIRE( constTestTable.GetItemAtKey( "second_key" ) == 666 );
            REQUIRE( constTestTable.GetItemAtKey( "third_key" ) == 5150 );
            REQUIRE_ASSERT( constTestTable.GetItemAtKey( "unknown_key" ) );
        }
    }

    TEST_CASE_METHOD( Fixture, "FindItemAtKey", "[primitive]" )
    {
        HashTable< DummyClass >::Iterator iterator;
        HashTable< DummyClass >::ConstIterator constIterator;
        const HashTable< DummyClass > &constTestTable = TestTable;

        iterator = TestTable.FindItemAtKey( "second_key" );
        REQUIRE( 666 == ( *iterator ).Value );

        constIterator = constTestTable.FindItemAtKey( "third_key" );
        REQUIRE( 5150 == ( *constIterator ).Value );

        REQUIRE( TestTable.FindItemAtKey( "unknown_key" ) == TestTable.end() );
        REQUIRE( constTestTable.FindItemAtKey( "unknown_key" ) == constTestTable.end() );
    }

    TEST_CASE_METHOD( Fixture, "hasItemAtKey", "[primitive]" )
    {
        REQUIRE( TestTable.HasItemAtKey( "first_key" ) );
        REQUIRE( TestTable.HasItemAtKey( "second_key" ) );
        REQUIRE( TestTable.HasItemAtKey( "third_key" ) );
        REQUIRE_FALSE( TestTable.HasItemAtKey( "unknown_key" ) );
    }

    TEST_CASE_METHOD( Fixture, "Insert", "[primitive]" )
    {
        DummyClass first{ 444, "fourth_key" };

        REQUIRE( 3 == TestTable.GetItemCount() );

        TestTable.Insert( &first );

        REQUIRE( 4 == TestTable.GetItemCount() );
        REQUIRE( 444 == TestTable.GetItemAtKey( "fourth_key" ) );

        REQUIRE_ASSERT( TestTable.Insert( &first ) );
    }

    TEST_CASE_METHOD( Fixture, "Clear", "[primitive]" )
    {
        REQUIRE( 3 == TestTable.GetItemCount() );
        REQUIRE_FALSE( TestTable.IsEmpty() );

        TestTable.Clear();
        REQUIRE( TestTable.IsEmpty() );
    }
}