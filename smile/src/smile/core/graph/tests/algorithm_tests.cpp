/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        algorithm_tests.cpp
 * @author      Zenn Geeraerts
 * @created     23 March 2026
 * @brief       Graph algorithm tests
 */
#include "smile/core/graph/algorithm.h"
#include <catch/catch.hpp>

namespace smile::graph
{
    TEST_CASE( "Topological sort" )
    {
        SECTION( "Simple linear DAG", "[TopologicalSort]" )
        {
            Graph graph{ true };

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();
            auto n2 = graph.AddNode();

            graph.AddConnection( n0, n1 );
            graph.AddConnection( n1, n2 );

            auto sorted = TopologicalSort( graph );

            REQUIRE( sorted.GetItemCount() == 3 );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n0 ) < std::find( sorted.begin(), sorted.end(), n1 ) );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n1 ) < std::find( sorted.begin(), sorted.end(), n2 ) );
        }

        SECTION( "DAG with branching", "[TopologicalSort]" )
        {
            Graph graph{ true };

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();
            auto n2 = graph.AddNode();
            auto n3 = graph.AddNode();

            graph.AddConnection( n0, n1 );
            graph.AddConnection( n0, n2 );
            graph.AddConnection( n1, n3 );
            graph.AddConnection( n2, n3 );

            auto sorted = TopologicalSort( graph );

            REQUIRE( sorted.GetItemCount() == 4 );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n0 ) < std::find( sorted.begin(), sorted.end(), n1 ) );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n0 ) < std::find( sorted.begin(), sorted.end(), n2 ) );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n1 ) < std::find( sorted.begin(), sorted.end(), n3 ) );
            REQUIRE( std::find( sorted.begin(), sorted.end(), n2 ) < std::find( sorted.begin(), sorted.end(), n3 ) );
        }

        SECTION( "Cycle detection triggers assert", "[TopologicalSort][.][should_fail]" )
        {
            Graph graph{ true };

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();

            graph.AddConnection( n0, n1 );
            graph.AddConnection( n1, n0 ); // cycle

            REQUIRE_ASSERT( TopologicalSort( graph ) );
        }
    }
}