/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/common/foundation/identifier.h"

#include <catch/catch.hpp>

namespace
{
    class DummyKey final
    {
      public:
        DummyKey() : Text{ nullptr }
        {
        }

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

    class NoDefaultConstructorDummyKey final
    {
      public:
        NoDefaultConstructorDummyKey() = delete;

        NoDefaultConstructorDummyKey( const char *text ) : Text{ text }
        {
        }

        bool operator==( const NoDefaultConstructorDummyKey &other ) const
        {
            return std::strcmp( Text, other.Text ) == 0;
        }

        bool operator!=( const NoDefaultConstructorDummyKey &other ) const
        {
            return !( *this == other );
        }

        const char *Text;
    };

    class DummyClass final
    {
      public:
        DummyClass() : Value{ 0 }
        {
            ++s_InstanceCount;
        }

        DummyClass( int value ) : Value{ value }
        {
            ++s_InstanceCount;
        }

        DummyClass( const DummyClass &other ) : Value{ other.Value }
        {
            ++s_InstanceCount;
        }

        ~DummyClass()
        {
            --s_InstanceCount;
        }

        bool operator==( const DummyClass &other ) const
        {
            return Value == other.Value;
        }

        bool operator!=( const DummyClass &other ) const
        {
            return Value != other.Value;
        }

        int Value;
        static smile::Count s_InstanceCount;
    };

    smile::Count DummyClass::s_InstanceCount = 0;

    class NoDefaultConstructorDummyClass final
    {
      public:
        NoDefaultConstructorDummyClass() = delete;
        NoDefaultConstructorDummyClass( int value ) : Value{ value }
        {
        }

        NoDefaultConstructorDummyClass( const NoDefaultConstructorDummyClass &other ) = default;

        ~NoDefaultConstructorDummyClass() = default;

        int Value;
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

    template <>
    struct hash< NoDefaultConstructorDummyKey >
    {
        size_t operator()( const NoDefaultConstructorDummyKey &key ) const
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
        Fixture()
        {
            InstanceCount = DummyClass::s_InstanceCount;
            TestMap.Insert( "first_key", DummyClass{ 166 } );
            TestMap.Insert( "second_key", DummyClass{ 666 } );
            TestMap.Insert( "third_key", DummyClass{ 5150 } );
        }

        ~Fixture()
        {
            TestMap.Clear();
            SM_ASSERT( InstanceCount == DummyClass::s_InstanceCount );
        }

        smile::primitive::HashMap< DummyKey, DummyClass > TestMap;
        smile::Count InstanceCount;
    };
}

namespace smile::primitive
{
    TEST_CASE_METHOD( Fixture, "HashMap::Constructors", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass > map;

        map.Insert( "first_key", DummyClass{ 166 } );
        map.Insert( "test2", DummyClass{ 1662 } );

        REQUIRE( map.GetItemAtKey( "first_key" ).Value == 166 );
        REQUIRE( map.GetItemAtKey( "test2" ).Value == 1662 );

        HashMap< NoDefaultConstructorDummyKey, NoDefaultConstructorDummyClass > noConstructorTable;

        noConstructorTable.Insert( "test2", NoDefaultConstructorDummyClass{ 1662 } );

        HashMap< DummyKey, DummyClass > tableFromInitializerList{
            { "test", 1 },
            { "test2", 2 },
            { "test3", 3 },
            { "test4", 4 },
        };

        REQUIRE( tableFromInitializerList.GetItemAtKey( "test" ).Value == 1 );
        REQUIRE( tableFromInitializerList.GetItemAtKey( "test2" ).Value == 2 );
        REQUIRE( tableFromInitializerList.GetItemAtKey( "test3" ).Value == 3 );
        REQUIRE( tableFromInitializerList.GetItemAtKey( "test4" ).Value == 4 );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::Iterator", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass >::Iterator iterator;
        Count iteratorCount;
        bool valueWasFound[3];

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            valueWasFound[index] = false;
        }

        iteratorCount = 0;

        iterator = TestMap.begin();

        do
        {
            switch ( ( *iterator ).Value.Value )
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
            iteratorCount++;
            ++iterator;

        } while ( iterator != TestMap.end() );

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            REQUIRE( valueWasFound[index] );
        }

        REQUIRE( 3 == iteratorCount );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::ConstIterator", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass >::ConstIterator iterator;
        const HashMap< DummyKey, DummyClass > &constTestMap = TestMap;
        Count iteratorCount;
        bool valueWasFound[3];

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            valueWasFound[index] = false;
        }

        iteratorCount = 0;

        iterator = constTestMap.begin();

        do
        {
            switch ( ( *iterator ).Value.Value )
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
            iteratorCount++;
            ++iterator;

        } while ( iterator != constTestMap.end() );

        for ( auto index : foundation::GetCountIterator( 3u ) )
        {
            REQUIRE( valueWasFound[index] );
        }

        REQUIRE( 3 == iteratorCount );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::IsEmpty", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass > emptyMap;

        REQUIRE( emptyMap.IsEmpty() );
        REQUIRE_FALSE( TestMap.IsEmpty() );

        TestMap.Clear();
        REQUIRE( TestMap.IsEmpty() );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::GetItemAtKey", "[primitive]" )
    {
        {
            REQUIRE( TestMap.GetItemAtKey( "first_key" ).Value == 166 );
            REQUIRE( TestMap.GetItemAtKey( "second_key" ).Value == 666 );
            REQUIRE( TestMap.GetItemAtKey( "third_key" ).Value == 5150 );
            REQUIRE_ASSERT( TestMap.GetItemAtKey( "unknown_key" ) );
        }

        {
            const HashMap< DummyKey, DummyClass > &constTestMap = TestMap;

            REQUIRE( constTestMap.GetItemAtKey( "first_key" ).Value == 166 );
            REQUIRE( constTestMap.GetItemAtKey( "second_key" ).Value == 666 );
            REQUIRE( constTestMap.GetItemAtKey( "third_key" ).Value == 5150 );
            REQUIRE_ASSERT( constTestMap.GetItemAtKey( "unknown_key" ) );
        }
    }

    TEST_CASE_METHOD( Fixture, "HashMap::operator[]", "[primitive]" )
    {
        {
            REQUIRE( TestMap["first_key"].Value == 166 );
            REQUIRE( TestMap["second_key"].Value == 666 );
            REQUIRE( TestMap["third_key"].Value == 5150 );
            REQUIRE_ASSERT( TestMap["unknown_key"] );
        }

        {
            const HashMap< DummyKey, DummyClass > &constTestMap = TestMap;

            REQUIRE( constTestMap["first_key"].Value == 166 );
            REQUIRE( constTestMap["second_key"].Value == 666 );
            REQUIRE( constTestMap["third_key"].Value == 5150 );
            REQUIRE_ASSERT( constTestMap["unknown_key"] );
        }
    }

    TEST_CASE_METHOD( Fixture, "HashMap::FindItemAtKey", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass >::Iterator iterator;
        HashMap< DummyKey, DummyClass >::ConstIterator constIterator;
        const HashMap< DummyKey, DummyClass > &constTestMap = TestMap;

        iterator = TestMap.FindItemAtKey( "first_key" );
        REQUIRE( iterator != TestMap.end() );
        REQUIRE( 166 == ( *iterator ).Value.Value );

        iterator = TestMap.FindItemAtKey( "second_key" );
        REQUIRE( iterator != TestMap.end() );
        REQUIRE( 666 == ( *iterator ).Value.Value );

        constIterator = constTestMap.FindItemAtKey( "third_key" );
        REQUIRE( constIterator != constTestMap.end() );
        REQUIRE( 5150 == ( *constIterator ).Value.Value );

        REQUIRE( TestMap.FindItemAtKey( "unknown_key" ) == TestMap.end() );
        REQUIRE( TestMap.FindItemAtKey( "unknown_key" ) == TestMap.end() );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::HasItemAtKey", "[primitive]" )
    {
        REQUIRE( TestMap.HasItemAtKey( "first_key" ) );
        REQUIRE( TestMap.HasItemAtKey( "second_key" ) );
        REQUIRE( TestMap.HasItemAtKey( "third_key" ) );
        REQUIRE_FALSE( TestMap.HasItemAtKey( "unknown_key" ) );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::Insert", "[primitive]" )
    {
        REQUIRE( 3 == TestMap.GetItemCount() );

        TestMap.Insert( "fourth_key", DummyClass{ 444 } );
        REQUIRE( 4 == TestMap.GetItemCount() );
        REQUIRE( 444 == TestMap.GetItemAtKey( "fourth_key" ).Value );

        REQUIRE_ASSERT( TestMap.Insert( "fourth_key", DummyClass{ 555 } ) );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::Clear", "[primitive]" )
    {
        REQUIRE( 3 == TestMap.GetItemCount() );

        TestMap.Clear();
        REQUIRE( TestMap.IsEmpty() );
    }

    TEST_CASE_METHOD( Fixture, "HashMap::EraseAndSetNext", "[primitive]" )
    {
        HashMap< DummyKey, DummyClass >::Iterator iterator;

        REQUIRE_ASSERT( TestMap.EraseAndSetNext( iterator ) );

        iterator = TestMap.begin();
        ++iterator;
        TestMap.EraseAndSetNext( iterator );

        REQUIRE( iterator != TestMap.end() );
        ++iterator;
        REQUIRE( iterator == TestMap.end() );

        REQUIRE( TestMap.GetItemCount() == DummyClass::s_InstanceCount );

        iterator = TestMap.begin();

        do
        {
            TestMap.EraseAndSetNext( iterator );

            REQUIRE( TestMap.GetItemCount() == DummyClass::s_InstanceCount );

        } while ( iterator != TestMap.end() );

        REQUIRE( TestMap.IsEmpty() );
    }
}