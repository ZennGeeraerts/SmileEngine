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
 *
 *              A DrawBin collects every entity handle whose draw call carries an identical
 *              SortKey.  The render pass resolves mesh, material, and pipeline per entity
 *              from the RenderWorld ECS — the bin itself carries only the sort order and
 *              the handles of contributing entities.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/renderer/draw/sort_key.h"
#include "smile/core/ecs/entity_handle.h"

namespace smile::graphic
{
    /**
     * A single binned entry produced during the queue phase.
     * All entries in the bin share the same SortKey.
     * Mesh, material, and pipeline are resolved per entity via the RenderWorld ECS
     * at execute time.
     */
    struct DrawBin final
    {
        SortKey Key;
        primitive::Vector< smile::ecs::EntityHandle > Entities;
    };
}
