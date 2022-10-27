#include <smile_engine/core/core.h>
#include <smile_engine/stl/sparse_set.h>
#include <thirdparty/catch.hpp>
#include <iostream>

using namespace smile;

TEST_CASE( "SparseSet" )
{
    SECTION( "SparseSet" )
    {
        stl::SparseSet< Uint32 > sparse_set{};

        sparse_set.insert( 15 );
        sparse_set.insert( 16 );
        sparse_set.insert( 17 );
        sparse_set.insert( 18 );

        REQUIRE( 0 == sparse_set.getIndex( 15 ) );
        REQUIRE( 1 == sparse_set.getIndex( 16 ) );
        REQUIRE( 2 == sparse_set.getIndex( 17 ) );
        REQUIRE( 3 == sparse_set.getIndex( 18 ) );

        for ( auto it{ sparse_set.begin() }; it != sparse_set.end(); ++it )
        {
            std::cout << *it << std::endl;
        }
        std::cout << '\n';

        sparse_set.erase( 16 );

        for ( auto it{ sparse_set.begin() }; it != sparse_set.end(); ++it )
        {
            std::cout << *it << std::endl;
        }

        /*REQUIRE( sparse_set.contains( 15 ) );
        REQUIRE( !sparse_set.contains( 16 ) );
        REQUIRE( sparse_set.contains( 17 ) );
        REQUIRE( sparse_set.contains( 18 ) );
        REQUIRE( sparse_set.getItemCount() == 3 );
        REQUIRE( 0 == sparse_set.getIndex( 15 ) );
        REQUIRE( 2 == sparse_set.getIndex( 17 ) );
        REQUIRE( 3 == sparse_set.getIndex( 18 ) );*/

        /*sparse_set.insert( 18 );

        REQUIRE( 16 == sparse_set.get( 0 ) );
        REQUIRE( 17 == sparse_set.get( 1 ) );
        REQUIRE( 18 == sparse_set.get( 2 ) );
        REQUIRE( sparse_set.getItemCount() == 3 );*/
    }
}