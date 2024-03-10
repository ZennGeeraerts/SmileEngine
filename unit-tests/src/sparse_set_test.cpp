#include <smile_engine/core/core.h>
#include <smile_engine/stl/sparse_set.h>
#include <catch/catch.hpp>
#include <iostream>

using namespace smile;

TEST_CASE( "SparseSet" )
{
    SECTION( "SparseSet" )
    {
        stl::SparseSet< Uint32 > sparseSet{};

        sparseSet.Insert( 15 );
        sparseSet.Insert( 16 );
        sparseSet.Insert( 17 );
        sparseSet.Insert( 18 );

        REQUIRE( 0 == sparseSet.GetIndex( 15 ) );
        REQUIRE( 1 == sparseSet.GetIndex( 16 ) );
        REQUIRE( 2 == sparseSet.GetIndex( 17 ) );
        REQUIRE( 3 == sparseSet.GetIndex( 18 ) );

        for ( auto it{ sparseSet.begin() }; it != sparseSet.end(); ++it )
        {
            std::cout << *it << std::endl;
        }
        std::cout << '\n';

        sparseSet.Erase( 16 );

        for ( auto it{ sparseSet.begin() }; it != sparseSet.end(); ++it )
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

    SECTION( "Sort" )
    {
        stl::SparseSet< Uint32 > sparseSet{};

        sparseSet.Insert( 16 );
        sparseSet.Insert( 17 );
        sparseSet.Insert( 18 );
        sparseSet.Insert( 15 );

        REQUIRE( sparseSet.GetElement( 0 ) == 16 );
        REQUIRE( sparseSet.GetElement( 1 ) == 17 );
        REQUIRE( sparseSet.GetElement( 2 ) == 18 );
        REQUIRE( sparseSet.GetElement( 3 ) == 15 );

        REQUIRE( sparseSet.GetIndex( 16 ) == 0 );
        REQUIRE( sparseSet.GetIndex( 17 ) == 1 );
        REQUIRE( sparseSet.GetIndex( 18 ) == 2 );
        REQUIRE( sparseSet.GetIndex( 15 ) == 3 );

        sparseSet.Sort( []( Uint32 left, Uint32 right ) { return left < right; } );

        REQUIRE( sparseSet.GetElement( 0 ) == 15 );
        REQUIRE( sparseSet.GetElement( 1 ) == 16 );
        REQUIRE( sparseSet.GetElement( 2 ) == 17 );
        REQUIRE( sparseSet.GetElement( 3 ) == 18 );

        REQUIRE( sparseSet.GetIndex( 15 ) == 0 );
        REQUIRE( sparseSet.GetIndex( 16 ) == 1 );
        REQUIRE( sparseSet.GetIndex( 17 ) == 2 );
        REQUIRE( sparseSet.GetIndex( 18 ) == 3 );
    }
}