/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smile/common/primitive/collection/array_view.h"

#include <catch/catch.hpp>

namespace smile::primitive
{
    TEST_CASE( "ArrayView", "[primitive]" )
    {
        SECTION( "Constructors" )
        {
            int dummy[] = { 1, 2, 3 };
            REQUIRE_NOTHROW( ArrayView< int >{} );
            REQUIRE_NOTHROW( ArrayView< int >{ dummy, 3 } );

            const int *pConstDummy = dummy;

            REQUIRE_NOTHROW( ArrayView< const int >{} );
            REQUIRE_NOTHROW( ArrayView< const int >{ pConstDummy, 3 } );

            ArrayView< int > empty;
            ArrayView< int > fromArray{ dummy, 3 };

            REQUIRE_NOTHROW( ArrayView< int >{ empty } );
            REQUIRE_NOTHROW( ArrayView< int >{ fromArray } );

            REQUIRE_NOTHROW( ArrayView< const int >{ empty } );
            REQUIRE_NOTHROW( ArrayView< const int >{ fromArray } );
        }

        SECTION( "Accessors" )
        {
            int dummy[] = { 1, 2, 3 };
            ArrayView< int > empty;
            ArrayView< int > fromArray{ dummy, 3 };

            const int *pConstDummy = dummy;

            ArrayView< const int > constEmpty;
            ArrayView< const int > constFromArray{ pConstDummy, 3 };

            REQUIRE( empty.GetItemCount() == 0 );
            REQUIRE( fromArray.GetItemCount() == 3 );
            REQUIRE( constEmpty.GetItemCount() == 0 );
            REQUIRE( constFromArray.GetItemCount() == 3 );

            REQUIRE( empty.GetData() == nullptr );
            REQUIRE( fromArray.GetData() == dummy );
            REQUIRE( constEmpty.GetData() == nullptr );
            REQUIRE( constFromArray.GetData() == dummy );
        }

        SECTION( "Index operator" )
        {
            int dummy[] = { 1, 2, 3 };
            ArrayView< int > empty;
            ArrayView< int > fromArray{ dummy, 3 };

            const int *pConstDummy = dummy;

            ArrayView< const int > constEmpty;
            ArrayView< const int > constFromArray( pConstDummy, 3 );

            REQUIRE_THROWS( empty[0] );
            REQUIRE_THROWS( empty[-1] );
            REQUIRE_THROWS( fromArray[-1] );
            REQUIRE_THROWS( fromArray[3] );
            REQUIRE( fromArray[0] == dummy[0] );
            REQUIRE( fromArray[1] == dummy[1] );
            REQUIRE( fromArray[2] == dummy[2] );

            REQUIRE_THROWS( constEmpty[0] );
            REQUIRE_THROWS( constEmpty[-1] );
            REQUIRE_THROWS( constFromArray[-1] );
            REQUIRE_THROWS( constFromArray[3] );
            REQUIRE( constFromArray[0] == dummy[0] );
            REQUIRE( constFromArray[1] == dummy[1] );
            REQUIRE( constFromArray[2] == dummy[2] );
        }
    }
}