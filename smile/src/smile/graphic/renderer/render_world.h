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
 * @file        render_world.h
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       Per-frame CPU-side submission container produced by the extract phase
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/uuid.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/core/ecs/ecs_engine.h"
#include "resource/frame_buffer.h"

namespace smile::graphic
{
    class ResourceManager;
    class MeshManager;
    class MaterialSystem;
    class View;
    class SpriteBatch;

    class RenderWorld final
    {
      public:
        void Initialize( ResourceManager &resourceManager );

        smile::ecs::EntityHandle CreateEntity();
        smile::ecs::EntityHandle CreateEntity( const primitive::UUID uuid );

        template < typename ComponentType, typename... Args >
        void AddComponent( smile::ecs::EntityHandle entity, Args &&...args )
        {
            m_ECSEngine.AddComponent< ComponentType >( entity, std::forward< Args >( args )... );
        }

        template < typename ComponentType, typename... Args >
        void AddOrReplaceComponent( smile::ecs::EntityHandle entity, Args &&...args )
        {
            m_ECSEngine.AddOrReplaceComponent< ComponentType >( entity, std::forward< Args >( args )... );
        }

        void SetViewport( const rhi::Viewport &viewport )
        {
            m_ViewportState.Viewports[0] = viewport;
        }

        void Prepare( ResourceManager &resourceManager,
            MeshManager &meshManager,
            MaterialSystem &materialSystem,
            View &view,
            SpriteBatch &spriteBatch );

      private:
        smile::ecs::ECSEngine m_ECSEngine;
        primitive::HashMap< primitive::UUID, smile::ecs::EntityHandle > m_EntityMap;
        rhi::ViewportState m_ViewportState;

        Framebuffer m_RenderTarget;
    };
}
