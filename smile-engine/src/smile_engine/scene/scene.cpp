#include "smpch.h"
#include "scene.h"

#include "components.h"
#include "smile_engine/graphic/renderer.h"
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
        ecsEngine.clear();
        //registry.clear();
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
        Entity entity{ ecsEngine.createEntity(), this };
        entity.addComponent< IDComponent >( uuid );
        entity.addComponent< TagComponent >( name );
        entity.addComponent< TransformComponent >();

        return entity;
    }

    void Scene::destroyEntity( Entity entity )
    {
        ecsEngine.destroyEntity( entity );
        //registry.destroy( entity );
    }

    void Scene::onRuntimeStart()
    {
        physics::PhysicsEngine::createScene();

        // Create physisc actors
        {
            auto view = ecsEngine.view< RigidbodyComponent >();
            for ( auto e : view )
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
            auto view = ecsEngine.view< RigidbodyComponent >();
            for ( auto e : view )
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

        graphic::Camera *main_camera = nullptr;
        DirectX::XMFLOAT4X4 camera_transform;
        {
            auto view = ecsEngine.view< TransformComponent, CameraComponent >();
            for ( auto entity : view )
            {
                const auto &[transform, camera] = ecsEngine.getComponents< TransformComponent, CameraComponent >( entity );

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
            graphic::Renderer::beginScene( *main_camera, camera_transform );

            {
                auto group = ecsEngine.group< MeshRendererComponent >( ecs::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = ecsEngine.getComponents< MeshRendererComponent, TransformComponent >( entity );
                    graphic::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = ecsEngine.group< StaticMeshComponent >( ecs::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = ecsEngine.getComponents< StaticMeshComponent, TransformComponent >( entity );
                    graphic::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = ecsEngine.group< SkinnedMeshComponent >( ecs::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[mesh, transform] = ecsEngine.getComponents< SkinnedMeshComponent, TransformComponent >( entity );

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

                    graphic::Renderer::submit( mesh, transform.getTransform() );
                }
            }
            {
                auto group = ecsEngine.group< BoxColliderComponent >( ecs::get< TransformComponent > );
                for ( auto entity : group )
                {
                    const auto &[boxCollider, transform] =
                        ecsEngine.getComponents< BoxColliderComponent, TransformComponent >( entity );
                    graphic::Renderer::submitWireframe( boxCollider, transform.getTransform() );
                }
            }

            graphic::Renderer::onRender();

            graphic::Renderer::endScene();
        }
    }

    void Scene::onUpdateSimulation( Timestep delta_time, graphic::EditorCamera &editor_camera )
    {
        physics::PhysicsEngine::simulate( delta_time );

        graphic::Renderer::beginScene( editor_camera );

        {
            auto group = ecsEngine.group< MeshRendererComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = ecsEngine.getComponents< MeshRendererComponent, TransformComponent >( entity );
                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< StaticMeshComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = ecsEngine.getComponents< StaticMeshComponent, TransformComponent >( entity );
                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< SkinnedMeshComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] =
                    ecsEngine.getComponents< SkinnedMeshComponent, TransformComponent >( entity );

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

                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< BoxColliderComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[box_collider, transform] =
                    ecsEngine.getComponents< BoxColliderComponent, TransformComponent >( entity );
                graphic::Renderer::submitWireframe( box_collider, transform.getTransform() );
            }
        }

        graphic::Renderer::onRender();

        graphic::Renderer::endScene();
    }

    void Scene::onUpdateEditor( Timestep delta_time, graphic::EditorCamera &editor_camera )
    {
        graphic::Renderer::beginScene( editor_camera );

        {
            auto group = ecsEngine.group< MeshRendererComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = ecsEngine.getComponents< MeshRendererComponent, TransformComponent >( entity );
                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< StaticMeshComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = ecsEngine.getComponents< StaticMeshComponent, TransformComponent >( entity );
                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< SkinnedMeshComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[mesh, transform] = ecsEngine.getComponents< SkinnedMeshComponent, TransformComponent >( entity );

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

                graphic::Renderer::submit( mesh, transform.getTransform() );
            }
        }
        {
            auto group = ecsEngine.group< BoxColliderComponent >( ecs::get< TransformComponent > );
            for ( auto entity : group )
            {
                const auto &[box_collider, transform] =
                    ecsEngine.getComponents< BoxColliderComponent, TransformComponent >( entity );
                graphic::Renderer::submitWireframe( box_collider, transform.getTransform() );
            }
        }

        graphic::Renderer::onRender();

        graphic::Renderer::endScene();
    }

    void Scene::onViewportResize( Uint32 width, Uint32 height )
    {
        viewportWidth = width;
        viewportHeight = height;

        auto view = ecsEngine.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = ecsEngine.getComponent< CameraComponent >( entity );
            if ( !cameraComponent.fixedAspectRatio )
            {
                cameraComponent.camera.setViewportSize( width, height );
            }
        }
    }

    Entity Scene::getPrimaryCameraEntity()
    {
        auto view = ecsEngine.view< CameraComponent >();
        for ( auto entity : view )
        {
            auto &camera_component = ecsEngine.getComponent< CameraComponent >( entity );
            if ( camera_component.primary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename ComponentType >
    static void
    CopyComponent( ecs::ECSEngine &dst, ecs::ECSEngine &src, const std::unordered_map< UUID, ecs::EntityHandleType > &entity_handle_map )
    {
        auto view = src.view< ComponentType >();
        for ( auto entity : view )
        {
            UUID uuid = src.getComponent< IDComponent >( entity ).id;
            SM_ASSERT( entity_handle_map.find( uuid ) != entity_handle_map.end(), "Scene::CopyComponent > uuid not found int enttMap" );
            ecs::EntityHandleType dst_entt_id = entity_handle_map.at( uuid );

            auto &component = src.getComponent< ComponentType >( entity );
            dst.addOrReplaceComponent< ComponentType >( dst_entt_id, component );
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

        std::unordered_map< UUID, ecs::EntityHandleType > entityMap{};

        auto &src_scene_registry = scene->ecsEngine;
        auto &dst_scene_registry = new_scene->ecsEngine;
        auto id_view = src_scene_registry.view< IDComponent >();
        for ( auto entity : id_view )
        {
            auto uuid = src_scene_registry.getComponent< IDComponent >( entity ).id;
            const auto &name = src_scene_registry.getComponent< TagComponent >( entity ).tag;
            Entity new_entity = new_scene->createEntity( uuid, name );
            entityMap[uuid] = static_cast< ecs::EntityHandleType >( new_entity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< TransformComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< MeshRendererComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< StaticMeshComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< SkinnedMeshComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< CameraComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< RigidbodyComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< BoxColliderComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< SphereColliderComponent >( dst_scene_registry, src_scene_registry, entityMap );
        CopyComponent< CapsuleColliderComponent >( dst_scene_registry, src_scene_registry, entityMap );

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
