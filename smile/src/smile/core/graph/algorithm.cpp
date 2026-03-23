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
 * @file        algorithm.cpp
 * @author      Zenn Geeraerts
 * @created     23 March 2026
 * @brief       Graph algorithms
 */
#include "smpch.h"
#include "algorithm.h"

namespace smile::graph
{
    primitive::Vector< Graph::NodeID > TopologicalSort( const Graph &graph )
    {
        SM_ASSERT( graph.IsDirectional() );

        const Count nodeCount = graph.GetNodeCount();
        primitive::Vector< Graph::NodeID > inDegree( nodeCount, 0 );

        for ( const Graph::NodeID nodeID : foundation::GetCountIterator( nodeCount ) )
        {
            for ( const Graph::Edge &neighbour : graph.GetConnections( nodeID ) )
            {
                ++inDegree[neighbour.To];
            }
        }

        primitive::Vector< Graph::NodeID > stack;
        stack.Reserve( nodeCount );

        for ( const Graph::NodeID nodeID : foundation::GetCountIterator( nodeCount ) )
        {
            if ( inDegree[nodeID] == 0 )
                stack.PushBack( nodeID );
        }

        primitive::Vector< Graph::NodeID > sorted{};

        while ( !stack.IsEmpty() )
        {
            auto currentNode = stack.GetLastItem();
            stack.PopBack();

            sorted.PushBack( currentNode );

            for ( const Graph::Edge &neighbor : graph.GetConnections( currentNode ) )
            {
                if ( --inDegree[neighbor.To] == 0 )
                {
                    stack.PushBack( neighbor.To );
                }
            }
        }

        SM_ASSERT_MSG( sorted.GetItemCount() == nodeCount, "TopologicalSort > Cycle detected or missing dependency" );

        return sorted;
    }
}