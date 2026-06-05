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
 * @file        draw_command_buffer.cpp
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       DrawCommandBuffer — Add, Sort, and Clear implementations.
 */
#include "smpch.h"
#include "smile/graphic/renderer/draw/draw_command_buffer.h"

#include <algorithm>

namespace smile::graphic
{
    void DrawCommandBuffer::Add( const BinKey binKey, const SortKey sortKey, const ecs::EntityHandle entity )
    {
        auto [it, inserted] = m_Bins.TryCreateItemAtKey( binKey );
        DrawBin &bin = it.GetItem();

        SM_ASSERT( inserted || bin.Key == binKey );

        bin.Items.PushBack( { sortKey, entity } );
        m_Sorted = false;
    }

    void DrawCommandBuffer::Sort()
    {
        if ( m_Sorted )
            return;

        m_SortedBins.Clear();

        for ( auto it = m_Bins.begin(); it != m_Bins.end(); ++it )
            m_SortedBins.PushBack( &it.GetItem() );

        std::sort( m_SortedBins.begin(),
            m_SortedBins.end(),
            []( const DrawBin *a, const DrawBin *b ) noexcept { return a->Key < b->Key; } );

        for ( DrawBin *bin : m_SortedBins )
            bin->Sort();

        m_Sorted = true;
    }

    void DrawCommandBuffer::Clear() noexcept
    {
        m_Bins.Clear();
        m_SortedBins.Clear();
        m_Sorted = false;
    }
}
