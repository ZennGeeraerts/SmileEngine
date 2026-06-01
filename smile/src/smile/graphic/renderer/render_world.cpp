#include "smpch.h"
#include "render_world.h"

#include "smile/core/world/ecs/id_component.h"
#include "smile/core/world/ecs/transform_component.h"
#include "ecs/sprite_renderer_component.h"
#include "sprite/sprite_batch.h"
#include "mesh/mesh_manager.h"
#include "material/material_system.h"
#include "resource/resource_manager.h"

namespace smile::graphic
{
    smile::ecs::EntityHandle RenderWorld::CreateEntity()
    {
        return CreateEntity( primitive::UUID{} );
    }

    smile::ecs::EntityHandle RenderWorld::CreateEntity( const primitive::UUID uuid )
    {
        const auto handle = m_ECSEngine.CreateEntity();
        m_ECSEngine.AddComponent< ecs::IDComponent >( handle, uuid );
        m_EntityMap[uuid] = handle;

        return handle;
    }

    void RenderWorld::Prepare( ResourceManager &resourceManager,
        MeshManager &meshManager,
        MaterialSystem &materialSystem,
        SpriteBatch &spriteBatch )
    {
        {
            auto group = m_ECSEngine.GetGroup< ecs::SpriteRendererComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[spriteRenderer, transform] =
                    m_ECSEngine.GetComponents< SpriteRendererComponent, world::ecs::TransformComponent >( entity );

                auto materialInstance = materialSystem.GetOrCreateMaterialInstance( spriteRenderer.Material );
                auto texture = resourceManager.GetOrCreateTexture2D( spriteRenderer.Texture );

                materialInstance.SetParameter( "Color", spriteRenderer.Color );
                materialInstance.SetTextureBinding( "Texture", texture, rhi::SamplerDescriptor{} );

                spriteBatch.Submit( transform.GetWorldMatrix(), materialInstance );
            }
        }

        {
            auto group = m_ECSEngine.GetGroup< ecs::MeshRendererComponent >(
                smile::ecs::g_Get< world::ecs::TransformComponent > );

            for ( auto entity : group )
            {
                const auto &[meshRenderer, transform] =
                    m_ECSEngine.GetComponents< ecs::MeshRendererComponent, world::ecs::TransformComponent >( entity );

                const auto mesh = meshManager.GetOrCreateMesh( meshRenderer.Mesh );
                const auto materialInstance = materialSystem.GetOrCreateMaterialInstance( meshRenderer.Material );

                AddOrReplaceComponent< Mesh >( entity, mesh );
                AddOrReplaceComponent< MaterialInstance >( entity, materialInstance );
            }
        }
    }
}