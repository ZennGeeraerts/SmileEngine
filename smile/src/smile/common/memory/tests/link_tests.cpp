/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___| |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        link_tests.cpp
 * @author      Zenn Geeraerts
 * @created     19 May 2026
 * @brief       Unit tests for memory::Link
 */
#include "smile/common/memory/link.h"
#include "smile/common/memory/ref.h"

#include <catch/catch.hpp>

namespace smile::memory
{
    struct LinkTestBase : public Counted
    {
        int value = 0;
        explicit LinkTestBase( int v = 0 ) : value{ v }
        {
        }
    };

    struct LinkTestDerived : public LinkTestBase
    {
        using Type = LinkTestDerived;
        int derivedValue = 0;
        explicit LinkTestDerived( int v = 0 ) : LinkTestBase{ v }, derivedValue{ v * 2 }
        {
        }
    };

    TEST_CASE( "Link default constructor produces null state", "[memory]" )
    {
        Link< LinkTestBase > link;

        REQUIRE( link.IsNull() );
        REQUIRE( !link );
        REQUIRE( !static_cast< bool >( link ) );
        REQUIRE( link.GetPointer() == nullptr );
    }

    TEST_CASE( "Link constructor from pointer", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 42 };
        Link< LinkTestBase > link{ object.GetPointer() };

        REQUIRE_FALSE( link.IsNull() );
        REQUIRE( static_cast< bool >( link ) );
        REQUIRE( link.GetPointer() == object.GetPointer() );
    }

    TEST_CASE( "Link copy constructor shares the same pointer", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 7 };
        Link< LinkTestBase > original{ object.GetPointer() };
        Link< LinkTestBase > copy{ original };

        REQUIRE( copy.GetPointer() == original.GetPointer() );
        REQUIRE_FALSE( copy.IsNull() );
    }

    TEST_CASE( "Link move constructor transfers pointer, source becomes null", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 7 };
        Link< LinkTestBase > original{ object.GetPointer() };
        Link< LinkTestBase > moved{ std::move( original ) };

        REQUIRE( moved.GetPointer() == object.GetPointer() );
        REQUIRE( original.IsNull() );
    }

    TEST_CASE( "Link copy assignment from another Link", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 3 };
        Link< LinkTestBase > a{ object.GetPointer() };
        Link< LinkTestBase > b;

        b = a;

        REQUIRE( b.GetPointer() == a.GetPointer() );
        REQUIRE_FALSE( b.IsNull() );
    }

    TEST_CASE( "Link move assignment transfers pointer, source becomes null", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 3 };
        Link< LinkTestBase > a{ object.GetPointer() };
        Link< LinkTestBase > b;

        b = std::move( a );

        REQUIRE( b.GetPointer() == object.GetPointer() );
        REQUIRE( a.IsNull() );
    }

    TEST_CASE( "Link assignment from raw pointer", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 5 };
        Link< LinkTestBase > link;

        link = object.GetPointer();

        REQUIRE( link.GetPointer() == object.GetPointer() );
    }

    TEST_CASE( "Link assignment from Ref", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 5 };
        Link< LinkTestBase > link;

        link = object;

        REQUIRE( link.GetPointer() == object.GetPointer() );
    }

    TEST_CASE( "Link equality operators between Links", "[memory]" )
    {
        Ref< LinkTestBase > a = new LinkTestBase{ 1 };
        Ref< LinkTestBase > b = new LinkTestBase{ 2 };

        Link< LinkTestBase > linkA{ a.GetPointer() };
        Link< LinkTestBase > linkACopy{ a.GetPointer() };
        Link< LinkTestBase > linkB{ b.GetPointer() };

        REQUIRE( linkA == linkACopy );
        REQUIRE_FALSE( linkA == linkB );
        REQUIRE( linkA != linkB );
        REQUIRE_FALSE( linkA != linkACopy );
    }

    TEST_CASE( "Link equality operators against raw pointer", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 9 };
        Link< LinkTestBase > link{ object.GetPointer() };
        Link< LinkTestBase > empty;

        REQUIRE( link == object.GetPointer() );
        REQUIRE( empty == nullptr );
        REQUIRE( link != nullptr );
        REQUIRE( empty != object.GetPointer() );
    }

    TEST_CASE( "Link Reset to null clears the pointer", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 1 };
        Link< LinkTestBase > link{ object.GetPointer() };

        REQUIRE_FALSE( link.IsNull() );

        link.Reset();

        REQUIRE( link.IsNull() );
        REQUIRE( link.GetPointer() == nullptr );
    }

    TEST_CASE( "Link Reset to new pointer updates the stored pointer", "[memory]" )
    {
        Ref< LinkTestBase > a = new LinkTestBase{ 1 };
        Ref< LinkTestBase > b = new LinkTestBase{ 2 };
        Link< LinkTestBase > link{ a.GetPointer() };

        link.Reset( b.GetPointer() );

        REQUIRE( link.GetPointer() == b.GetPointer() );
    }

    TEST_CASE( "Link dereference operator accesses the underlying object", "[memory]" )
    {
        Ref< LinkTestBase > object = new LinkTestBase{ 99 };
        Link< LinkTestBase > link{ object.GetPointer() };

        REQUIRE( ( *link ).value == 99 );
        REQUIRE( link->value == 99 );
    }

    TEST_CASE( "Link GetObject casts to derived type", "[memory]" )
    {
        Ref< LinkTestDerived > source = new LinkTestDerived{ 10 };
        Link< LinkTestBase > link{ source.GetPointer() };

        const auto &derived = link.GetObject< LinkTestDerived >();

        REQUIRE( derived.value == 10 );
        REQUIRE( derived.derivedValue == 20 );
    }
}
