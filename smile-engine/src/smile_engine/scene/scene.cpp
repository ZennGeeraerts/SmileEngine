#include "smpch.h"
#include "scene.h"

#include "components.h"
#include "smile_engine/renderer/renderer.h"
#include "smile_engine/core/application.h"
#include "smile_engine/physics/physics_engine.h"

#include "entity.h"

namespace smile::scene
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        registry.clear();
    }

    Entity Scene::createEntity()
    {
        return createEntity( "Entity" );
    }

    Entity Scene::createEntity( const std::string &name )
    {
        return createEntity( UUID{}, name );
    }

    Entity Scene::createEntity( UUID uuid, const std::string &name )
    {
        Entity entity{ registry.create(), this };
        entity.addComponent< IDComponent >( uuid );
        entity.addComponent< TagComponent >( name );
        entity.addComponent< TransformComponent >();

        return entity;
    }

    void Scene::destroyEntity( Entity entity )
    {
        registry.destroy( entity );
    }

    void Scene::onRuntimeStart()
    {
        physics::PhysicsEngine::createScene();

        // Create physisc actors
        {
            const auto &view = registry.view< RigidbodyComponent >();
            for ( auto &e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::createActor( entity );
            }
        }
    }

    void Scene::onRuntimeStop()
    {
        physics::PhysicsEngine::destroyScene();
    }

    void Scene::onSimulationStart()
    {
        physics::PhysicsEngine::createScene();

        // Create physisc actors
        {
            const auto &view = registry.view< RigidbodyComponent >();
            for ( auto &e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::createActor( entity );
            }
        }
    }

    void Scene::onSimulationStop()
    {
        physics::PhysicsEngine::destroyScene();
    }

    void Scene::onUpdateRuntime( Timestep delta_time )
    {
        physics::PhysicsEngine::simulate( delta_time );

        renderer::Camera *main_camera = nullptr;
        DirectX::XMFLOAT4X4 camera_transform;
        {
            auto view = registry.view< TransformComponent, CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] = view.get< TransformComponent, CameraComponent >( entity );

                if ( camera.primary )
                {
                    main_camera = &camera.camera;
                    camera_transform = transform.getTransform();
                    break;
                }
            }
        }

        if ( main_camera )
        {
            renderer::Renderer::beginScene( *main_camera, camera_transform );

            {
                auto group = registry.group< MeshRendererComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< MeshRendererComponent, TransformComponent >( entity );
                    renderer::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = registry.group< StaticMeshComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< StaticMeshComponent, TransformComponent >( entity );
                    renderer::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = registry.group< SkinnedMeshComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = group.get< SkinnedMeshComponent, TransformComponent >( entity );

                    for ( auto &animator : mesh.animators )
                    {
                        animator.onUpdate( delta_time );
                        const auto &bone_transforms = animator.getBoneTransforms();
                        for ( const auto &material : mesh.materials )
                        {
                            if ( animator.isPlaying() )
                                material->getShader()->uploadMat4Array( "Bones", bone_transforms );
                        }
                    }

                    renderer::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = registry.group< BoxColliderComponent >( entt::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[boxCollider, transform] =
                        group.get< BoxColliderComponent, TransformComponent >( entity );
                    renderer::Renderer::submitWireframe( boxCollider, transform.getTransform() );
                }
            }

            renderer::Renderer::onRender();

            renderer::Renderer::endScene();
        }
    }

    void Scene::onUpdateSimulation( Timestep delta_time, renderer::EditorCamera &editor_camera )
    {
        physics::PhysicsEngine::simulate( delta_time );

        renderer::Renderer::beginScene( editor_camera );

        {
            auto group = registry.group< MeshRendererComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< MeshRendererComponent, TransformComponent >( entity );
                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< StaticMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< StaticMeshComponent, TransformComponent >( entity );
                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< SkinnedMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< SkinnedMeshComponent, TransformComponent >( entity );

                for ( auto &animator : mesh.animators )
                {
                    animator.onUpdate( delta_time );
                    const auto &bone_transforms = animator.getBoneTransforms();
                    for ( const auto &material : mesh.materials )
                    {
                        if ( animator.isPlaying() )
                            material->getShader()->uploadMat4Array( "Bones", bone_transforms );
                    }
                }

                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< BoxColliderComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[box_collider, transform] = group.get< BoxColliderComponent, TransformComponent >( entity );
                renderer::Renderer::submitWireframe( box_collider, transform.getTransform() );
            }
        }

        renderer::Renderer::onRender();

        renderer::Renderer::endScene();
    }

    void Scene::onUpdateEditor( Timestep delta_time, renderer::EditorCamera &editor_camera )
    {
        renderer::Renderer::beginScene( editor_camera );

        {
            auto group = registry.group< MeshRendererComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< MeshRendererComponent, TransformComponent >( entity );
                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< StaticMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< StaticMeshComponent, TransformComponent >( entity );
                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< SkinnedMeshComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = group.get< SkinnedMeshComponent, TransformComponent >( entity );

                for ( auto &animator : mesh.animators )
                {
                    animator.onUpdate( delta_time );
                    const auto &bone_transforms = animator.getBoneTransforms();
                    for ( const auto &material : mesh.materials )
                    {
                        if ( animator.isPlaying() )
                            material->getShader()->uploadMat4Array( "Bones", bone_transforms );
                    }
                }

                renderer::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = registry.group< BoxColliderComponent >( entt::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[box_collider, transform] = group.get< BoxColliderComponent, TransformComponent >( entity );
                renderer::Renderer::submitWireframe( box_collider, transform.getTransform() );
            }
        }

        renderer::Renderer::onRender();

        renderer::Renderer::endScene();
    }

    void Scene::onViewportResize( Uint32 width, Uint32 height )
    {
        viewportWidth = width;
        viewportHeight = height;

        auto view = registry.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = view.get< CameraComponent >( entity );
            if ( !cameraComponent.fixedAspectRatio )
            {
                cameraComponent.camera.setViewportSize( width, height );
            }
        }
    }

    Entity Scene::getPrimaryCameraEntity()
    {
        auto view = registry.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &camera_component = view.get< CameraComponent >( entity );
            if ( camera_component.primary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename ComponentType >
    static void
    CopyComponent( entt::registry &dst, entt::registry &src, const std::unordered_map< UUID, entt::entity > &enttMap )
    {
        auto view = src.view< ComponentType >();
        for ( auto entity : view )
        {
            UUID uuid = src.get< IDComponent >( entity ).id;
            SM_ASSERT( enttMap.find( uuid ) != enttMap.end(), "Scene > CopyComponent > uuid not found int enttMap" );
            entt::entity dst_entt_id = enttMap.at( uuid );

            auto &component = src.get< ComponentType >( entity );
            dst.emplace_or_replace< ComponentType >( dst_entt_id, component );
        }
    }

    template < typename ComponentType >
    static void CopyComponentIfExists( Entity dst, Entity src )
    {
        if ( src.hasComponent< ComponentType >() )
            dst.addOrReplaceComponent< ComponentType >( src.getComponent< ComponentType >() );
    }

    Ref< Scene > Scene::copy( const Ref< Scene > &scene )
    {
        Ref< Scene > new_scene = createRef< Scene >();

        new_scene->viewportWidth = scene->viewportWidth;
        new_scene->viewportHeight = scene->viewportHeight;

        std::unordered_map< UUID, entt::entity > enttMap{};

        auto &src_scene_registry = scene->registry;
        auto &dst_scene_registry = new_scene->registry;
        auto id_view = src_scene_registry.view< IDComponent >();
        for ( auto entity : id_view )
        {
            auto uuid = src_scene_registry.get< IDComponent >( entity ).id;
            const auto &name = src_scene_registry.get< TagComponent >( entity ).tag;
            Entity new_entity = new_scene->createEntity( uuid, name );
            enttMap[uuid] = static_cast< entt::entity >( new_entity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< TransformComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< MeshRendererComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< StaticMeshComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< SkinnedMeshComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< CameraComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< RigidbodyComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< BoxColliderComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< SphereColliderComponent >( dst_scene_registry, src_scene_registry, enttMap );
        CopyComponent< CapsuleColliderComponent >( dst_scene_registry, src_scene_registry, enttMap );

        return new_scene;
    }

    void Scene::duplicateEntity( Entity entity )
    {
        Entity new_entity = createEntity( entity.getName() );

        CopyComponentIfExists< TransformComponent >( new_entity, entity );
        CopyComponentIfExists< MeshRendererComponent >( new_entity, entity );
        CopyComponentIfExists< StaticMeshComponent >( new_entity, entity );
        CopyComponentIfExists< SkinnedMeshComponent >( new_entity, entity );
        CopyComponentIfExists< CameraComponent >( new_entity, entity );
        CopyComponentIfExists< RigidbodyComponent >( new_entity, entity );
        CopyComponentIfExists< BoxColliderComponent >( new_entity, entity );
        CopyComponentIfExists< SphereColliderComponent >( new_entity, entity );
        CopyComponentIfExists< CapsuleColliderComponent >( new_entity, entity );
    }

    template < typename ComponentType >
    void Scene::onComponentAdded( Entity entity, ComponentType &component )
    {
        static_assert( false );
    }

    template <>
    void Scene::onComponentAdded< IDComponent >( Entity entity, IDComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< TagComponent >( Entity entity, TagComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< TransformComponent >( Entity entity, TransformComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< CameraComponent >( Entity entity, CameraComponent &component )
    {
        component.camera.setViewportSize( viewportWidth, viewportHeight );
    }

    template <>
    void Scene::onComponentAdded< MeshRendererComponent >( Entity entity, MeshRendererComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< StaticMeshComponent >( Entity entity, StaticMeshComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< SkinnedMeshComponent >( Entity entity, SkinnedMeshComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< RigidbodyComponent >( Entity entity, RigidbodyComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< BoxColliderComponent >( Entity entity, BoxColliderComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< SphereColliderComponent >( Entity entity, SphereColliderComponent &component )
    {
    }

    template <>
    void Scene::onComponentAdded< CapsuleColliderComponent >( Entity entity, CapsuleColliderComponent &component )
    {
    }
}
