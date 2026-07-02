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
#include "material/material.h"
#include "material/asset/material_instance_asset.h"
#include "resource/frame_buffer.h"
#include "resource/graphics_pipeline.h"
#include "shader/binding_layout.h"
#include "shader/binding_set.h"
#include "shader/constant_buffer.h"
#include "draw/binned_command_buffer.h"
#include "draw/opaque3d.h"

namespace smile::graphic
{
    class ResourceManager;
    class MeshManager;
    class MaterialSystem;
    class View;
    class RenderContext;

    class RenderWorld final
    {
      public:
        smile::ecs::EntityHandle CreateEntity();
        smile::ecs::EntityHandle CreateEntity( const primitive::UUID uuid );

        template < typename ComponentType, typename... Args >
        ComponentType &AddComponent( smile::ecs::EntityHandle entity, Args &&...args )
        {
            return m_ECSEngine.AddComponent< ComponentType >( entity, std::forward< Args >( args )... );
        }

        template < typename ComponentType, typename... Args >
        ComponentType &AddOrReplaceComponent( smile::ecs::EntityHandle entity, Args &&...args )
        {
            return m_ECSEngine.AddOrReplaceComponent< ComponentType >( entity, std::forward< Args >( args )... );
        }

        template < typename ComponentType >
        const ComponentType &GetComponent( smile::ecs::EntityHandle entity ) const
        {
            return m_ECSEngine.GetComponent< ComponentType >( entity );
        }

        template < typename ComponentType >
        ComponentType &GetComponent( smile::ecs::EntityHandle entity )
        {
            return m_ECSEngine.GetComponent< ComponentType >( entity );
        }

        template < typename... Components >
        auto GetView()
        {
            return m_ECSEngine.template GetView< Components... >();
        }

        template < typename... Owned, typename... Get >
        auto GetGroup( smile::ecs::ComponentList< Get... > get = {} )
        {
            return m_ECSEngine.template GetGroup< Owned..., Get... >( get );
        }

        void Prepare( RenderContext &renderContext,
            ResourceManager &resourceManager,
            MeshManager &meshManager,
            MaterialSystem &materialSystem );

        void Enqueue( ViewBinnedCommandBuffers< Opaque3d > &opaqueCommandBuffers );

      private:
        void PrepareRenderable( smile::ecs::EntityHandle entity,
            const BindingLayout &viewBindingLayout,
            const MeshSource::Ref &meshSource,
            const MaterialInstanceAsset::ConstRef &materialInstanceAsset,
            ResourceManager &resourceManager,
            MeshManager &meshManager,
            MaterialSystem &materialSystem );

        const MaterialInstance &PrepareMaterial( smile::ecs::EntityHandle entity,
            const MaterialInstanceAsset::ConstRef &materialInstanceAsset,
            MaterialSystem &materialSystem );

        void PrepareMesh( smile::ecs::EntityHandle entity,
            const MeshSource::Ref &meshSource,
            const rhi::BufferLayout &vertexLayout,
            MeshManager &meshManager );

        void PreparePipeline( smile::ecs::EntityHandle entity,
            const rhi::BufferLayout &vertexLayout,
            const BindingLayout &viewBindingLayout,
            const Material &material,
            const MaterialData &materialData,
            ResourceManager &resourceManager );

        smile::ecs::ECSEngine m_ECSEngine;
        primitive::HashMap< primitive::UUID, smile::ecs::EntityHandle > m_EntityMap;
    };
}
