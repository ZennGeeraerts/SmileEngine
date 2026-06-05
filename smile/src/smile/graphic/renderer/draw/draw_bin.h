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
 * @file        draw_bin.h
 * @author      Zenn Geeraerts
 * @created     03 June 2026
 * @brief       Binned draw entry grouping entity instances that share the same sort key.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "sort_key.h"
#include "bin_key.h"
#include "smile/core/ecs/entity_handle.h"

namespace smile::graphic
{
    struct BinItem final
    {
        SortKey Key;
        smile::ecs::EntityHandle Entity;
    };

    struct DrawBin final
    {
        BinKey Key;
        primitive::Vector< BinItem > Items;

        void Sort() noexcept
        {
            std::sort( Items.begin(), Items.end(), []( const BinItem &a, const BinItem &b ) { return a.Key < b.Key; } );
        }
    };
}
