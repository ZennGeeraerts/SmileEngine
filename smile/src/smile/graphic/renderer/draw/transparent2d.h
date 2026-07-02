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
 * @file        transparent2d.h
 * @author      Zenn Geeraerts
 * @created     2 July 2026
 * @brief       Transparent 2D draw command for the sorted command buffer.
 */
#pragma once

#include "sort_key.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/core/ecs/entity_handle.h"

namespace smile::graphic
{
    class Transparent2d final
    {
      public:
        constexpr Transparent2d( smile::ecs::EntityHandle entity,
            const GraphicsPipeline &pipeline,
            const MaterialInstance &materialInstance,
            float depth ) noexcept
            : m_SortKey{ sort_key::EncodeTransparent( pipeline.GetHandle().GetIndex(),
                  materialInstance.GetHandle().GetIndex(),
                  depth ) },
              m_Entity{ entity }
        {
        }

        SortKey GetSortKey() const noexcept
        {
            return m_SortKey;
        }

        smile::ecs::EntityHandle GetEntity() const noexcept
        {
            return m_Entity;
        }

      private:
        SortKey m_SortKey;
        smile::ecs::EntityHandle m_Entity;
    };
}