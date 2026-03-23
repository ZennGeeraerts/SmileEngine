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
 * @file        graph.h
 * @author      Zenn Geeraerts
 * @created     23 March 2026
 * @brief       Graph data structure
 */
#pragma once

#include "smile/common/primitive/collection/vector.h"

namespace smile::graph
{
    class Graph final
    {
      public:
        using NodeID = Uint32;

        struct Edge final
        {
            NodeID From;
            NodeID To;
        };

        explicit Graph( bool isDirectional = false ) noexcept : m_IsDirectional{ isDirectional }
        {
        }

        NodeID AddNode()
        {
            m_AdjacencyList.EmplaceBack();
            return m_AdjacencyList.GetItemCount() - 1;
        }

        void AddConnection( NodeID from, NodeID to )
        {
            m_AdjacencyList[from].EmplaceBack( from, to );

            if ( !m_IsDirectional )
            {
                m_AdjacencyList[to].EmplaceBack( to, from );
            }
        }

        [[nodiscard]] const primitive::Vector< Edge > &GetConnections( NodeID node ) const
        {
            return m_AdjacencyList[node];
        }

        [[nodiscard]] Count GetNodeCount() const noexcept
        {
            return m_AdjacencyList.GetItemCount();
        }

        [[nodiscard]] bool IsDirectional() const noexcept
        {
            return m_IsDirectional;
        }

      private:
        primitive::Vector< primitive::Vector< Edge > > m_AdjacencyList;
        bool m_IsDirectional;
    };
}