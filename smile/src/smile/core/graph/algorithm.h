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
 * @file        algorithm.h
 * @author      Zenn Geeraerts
 * @created     23 March 2026
 * @brief       Graph algorithms
 */
#pragma once

#include "graph.h"

namespace smile::graph
{
    primitive::Vector< Graph::NodeID > TopologicalSort( const Graph &graph );
}