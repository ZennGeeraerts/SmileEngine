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
 * @file        graph_tests.cpp
 * @author      Zenn Geeraerts
 * @created     23 March 2026
 * @brief       Graph data structure tests
 */
#include "smile/core/graph/graph.h"
#include <catch/catch.hpp>

namespace smile::graph
{
    TEST_CASE( "Graph" )
    {
        SECTION( "AddNode increases node count", "[Graph]" )
        {
            Graph graph;

            REQUIRE( graph.GetNodeCount() == 0 );

            auto node0 = graph.AddNode();
            REQUIRE( node0 == 0 );
            REQUIRE( graph.GetNodeCount() == 1 );

            auto node1 = graph.AddNode();
            REQUIRE( node1 == 1 );
            REQUIRE( graph.GetNodeCount() == 2 );
        }

        SECTION( "AddConnection undirected", "[Graph]" )
        {
            Graph graph{ false };

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();

            graph.AddConnection( n0, n1 );

            const auto &connections0 = graph.GetConnections( n0 );
            REQUIRE( connections0[0].From == n0 );
            REQUIRE( connections0[0].To == n1 );

            const auto &connections1 = graph.GetConnections( n1 );
            REQUIRE( connections1.GetItemCount() == 1 );
            REQUIRE( connections1[0].From == n1 );
            REQUIRE( connections1[0].To == n0 );
        }

        SECTION( "AddConnection directed", "[Graph]" )
        {
            Graph graph{ true };

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();

            graph.AddConnection( n0, n1 );

            const auto &connections0 = graph.GetConnections( n0 );
            REQUIRE( connections0.GetItemCount() == 1 );
            REQUIRE( connections0[0].From == n0 );
            REQUIRE( connections0[0].To == n1 );

            const auto &connections1 = graph.GetConnections( n1 );
            REQUIRE( connections1.GetItemCount() == 0 );
        }

        SECTION( "Multiple connections", "[Graph]" )
        {
            Graph graph;

            auto n0 = graph.AddNode();
            auto n1 = graph.AddNode();
            auto n2 = graph.AddNode();

            graph.AddConnection( n0, n1 );
            graph.AddConnection( n0, n2 );

            const auto &connections0 = graph.GetConnections( n0 );
            REQUIRE( connections0.GetItemCount() == 2 );

            REQUIRE( ( connections0[0].To == n1 || connections0[0].To == n2 ) );
            REQUIRE( ( connections0[1].To == n1 || connections0[1].To == n2 ) );

            const auto &connections1 = graph.GetConnections( n1 );
            REQUIRE( connections1.GetItemCount() == 1 );
            REQUIRE( connections1[0].To == n0 );

            const auto &connections2 = graph.GetConnections( n2 );
            REQUIRE( connections2.GetItemCount() == 1 );
            REQUIRE( connections2[0].To == n0 );
        }
    }
}