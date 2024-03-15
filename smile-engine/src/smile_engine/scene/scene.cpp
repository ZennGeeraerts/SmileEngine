/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "components.h"
#include "smile_engine/graphic/render_engine.h"
#include "smile_engine/physics/physics_engine.h"
#include "smile_engine/scripting/script_engine.h"

#include "entity.h"

#include "smile_engine/graphic/renderer/render_pass/forward_render_pass.h"
#include "smile_engine/graphic/renderer/render_pass/wireframe_render_pass.h"
#include "smile_engine/graphic/renderer/render_pass/debug_render_pass.h"
#include "smile_engine/graphic/renderer/render_pass/render_pass_2d.h"

#include "transform_system.h"
#include "smile_engine/graphic/animation/animation_system.h"
#include "smile_engine/graphic/camera/camera_system.h"

#include "smile_engine/ecs/relationship.h"

namespace smile::scene
{
    Scene::Scene()
    {
        m_pTransformSystem = CreateScope< TransformSystem >( &m_ECSEngine );
        //m_pTransformSystem->m_pECSEngine = &m_ECSEngine;

        m_ECSEngine.AddSystem( new graphic::AnimationSystem{} );
        m_ECSEngine.AddSystem( new graphic::CameraSystem{} );
    }

    Scene::~Scene()
    {
        m_ECSEngine.Clear();
    }

    Entity Scene::CreateEntity()
    {
        return CreateEntity( "Entity" );
    }

    Entity Scene::CreateEntity( const std::string &name )
    {
        return CreateEntity( UUID{}, name );
    }

    Entity Scene::CreateEntity( UUID uuid, const std::string &name )
    {
        Entity entity{ m_ECSEngine.CreateEntity(), this };
        entity.AddComponent< IDComponent >( uuid );
        entity.AddComponent< TagComponent >( name );
        entity.AddComponent< TransformComponent >();

        m_EntityMap[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity( Entity entity )
    {
        if ( physics::PhysicsEngine::IsPhysicsActor( entity ) )
            physics::PhysicsEngine::RemoveActor( entity );

        auto pRelationship = entity.TryGetComponent< ecs::Relationship >();
        if ( pRelationship )
        {
            if ( pRelationship->Parent )
            {
                Entity parent{ pRelationship->Parent, this };
                parent.RemoveChild( entity );
            }

            auto currentChildHandle = pRelationship->First;
            while ( currentChildHandle )
            {
                Entity currentChild{ currentChildHandle, this };
                currentChildHandle = currentChild.GetComponent< ecs::Relationship >().Next;

                DestroyEntity( currentChild );
            }
        }

        m_EntityMap.erase( entity.GetUUID() );
        m_ECSEngine.DestroyEntity( entity );
    }

    void Scene::OnOpen()
    {
        graphic::RenderEngine::ClearRenderPasses();
        graphic::RenderEngine::AddRenderPass( new graphic::ForwardRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::WireframeRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::DebugRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::RenderPass2D{ m_ECSEngine } );
    }

    void Scene::OnRuntimeStart()
    {
        OnSimulationStart();

        // Scripting
        {
            scripting::ScriptEngine::OnRuntimeStart( this );

            // Instantiate all script entities
            auto view = m_ECSEngine.GetView< ScriptComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                scripting::ScriptEngine::OnCreateEntity( entity );
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        physics::PhysicsEngine::DestroyScene();
        scripting::ScriptEngine::OnRuntimeStop();
    }

    void Scene::OnSimulationStart()
    {
        physics::PhysicsEngine::CreateScene();

        // Create physisc actors
        {
            auto view = m_ECSEngine.GetView< RigidbodyComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::CreateActor( entity );
            }
        }
        // Create character controllers
        {
            auto view = m_ECSEngine.GetView< CharacterControllerComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                physics::PhysicsEngine::CreateCharacterController( entity );
            }
        }
    }

    void Scene::OnSimulationStop()
    {
        physics::PhysicsEngine::DestroyScene();
    }

    void Scene::OnUpdateRuntime( Timestep deltaTime )
    {
        m_pTransformSystem->OnUpdate( deltaTime );

        auto view = m_ECSEngine.GetView< ScriptComponent >();
        for ( auto e : view )
        {
            Entity entity = { e, this };
            scripting::ScriptEngine::OnUpdateEntity( entity, deltaTime );
        }

        physics::PhysicsEngine::Simulate( deltaTime );

        m_ECSEngine.OnUpdate( deltaTime );

        graphic::RenderEngine::OnRender();
    }

    void Scene::OnUpdateSimulation( Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_pTransformSystem->OnUpdate( deltaTime );

        physics::PhysicsEngine::Simulate( deltaTime );
        graphic::RenderEngine::OnRender( editorCamera );
    }

    void Scene::OnUpdateEditor( Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_pTransformSystem->OnUpdate( deltaTime );

        graphic::RenderEngine::OnRender( editorCamera );
    }

    void Scene::OnViewportResize( Uint32 width, Uint32 height )
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_ECSEngine.GetView< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = m_ECSEngine.GetComponent< CameraComponent >( entity );
            if ( !cameraComponent.HasFixedAspectRatio )
            {
                cameraComponent.Camera.SetViewportSize( width, height );
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_ECSEngine.GetView< CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = m_ECSEngine.GetComponent< CameraComponent >( entity );
            if ( cameraComponent.IsPrimary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename... ComponentType >
    static void CopyComponent( ecs::ECSEngine &dst,
        ecs::ECSEngine &src,
        const std::unordered_map< UUID, ecs::EntityHandleType > &entityHandleMap )
    {
        (
            [&]()
            {
                auto view = src.GetView< ComponentType >();
                for ( auto entity : view )
                {
                    UUID uuid = src.GetComponent< IDComponent >( entity ).ID;
                    SM_ASSERT( entityHandleMap.find( uuid ) != entityHandleMap.end(),
                        "Scene::CopyComponent > uuid not found int enttMap" );
                    ecs::EntityHandleType dstHandleID = entityHandleMap.at( uuid );

                    auto &component = src.GetComponent< ComponentType >( entity );
                    dst.AddOrReplaceComponent< ComponentType >( dstHandleID, component );
                }
            }(),
            ... );
    }

    template < typename... ComponentType >
    static void CopyComponent( ComponentGroup< ComponentType... >,
        ecs::ECSEngine &dst,
        ecs::ECSEngine &src,
        const std::unordered_map< UUID, ecs::EntityHandleType > &entityHandleMap )
    {
        CopyComponent< ComponentType... >( dst, src, entityHandleMap );
    }

    template < typename... ComponentType >
    static void CopyComponentIfExists( Entity dst, Entity src )
    {
        (
            [&]()
            {
                if ( src.HasComponent< ComponentType >() )
                    dst.AddOrReplaceComponent< ComponentType >( src.GetComponent< ComponentType >() );
            }(),
            ... );
    }

    template < typename... ComponentType >
    static void CopyComponentIfExists( ComponentGroup< ComponentType... >, Entity dst, Entity src )
    {
        CopyComponentIfExists< ComponentType... >( dst, src );
    }

    Ref< Scene > Scene::Copy( const Ref< Scene > &scene )
    {
        Ref< Scene > pNewScene = CreateRef< Scene >();

        pNewScene->m_ViewportWidth = scene->m_ViewportWidth;
        pNewScene->m_ViewportHeight = scene->m_ViewportHeight;

        std::unordered_map< UUID, ecs::EntityHandleType > entityMap{};

        auto &srcSceneEngine = scene->m_ECSEngine;
        auto &dstSceneEngine = pNewScene->m_ECSEngine;
        auto idView = srcSceneEngine.GetView< IDComponent >();
        for ( auto entity : idView )
        {
            auto uuid = srcSceneEngine.GetComponent< IDComponent >( entity ).ID;
            const auto &name = srcSceneEngine.GetComponent< TagComponent >( entity ).Tag;
            Entity newEntity = pNewScene->CreateEntity( uuid, name );
            entityMap[uuid] = static_cast< ecs::EntityHandleType >( newEntity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< ecs::Relationship >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent( AllComponents{}, dstSceneEngine, srcSceneEngine, entityMap );

        return pNewScene;
    }

    void Scene::DuplicateEntity( Entity entity )
    {
        Entity newEntity = CreateEntity( entity.GetName() );

        CopyComponentIfExists< ecs::Relationship >( newEntity, entity );
        CopyComponentIfExists< TransformComponent >( newEntity, entity );
        CopyComponentIfExists< MeshRendererComponent >( newEntity, entity );
        CopyComponentIfExists< SkinnedMeshRendererComponent >( newEntity, entity );
        CopyComponentIfExists< AnimatorComponent >( newEntity, entity );
        CopyComponentIfExists< CameraComponent >( newEntity, entity );
        CopyComponentIfExists< ScriptComponent >( newEntity, entity );
        CopyComponentIfExists< RigidbodyComponent >( newEntity, entity );
        CopyComponentIfExists< BoxColliderComponent >( newEntity, entity );
        CopyComponentIfExists< SphereColliderComponent >( newEntity, entity );
        CopyComponentIfExists< CapsuleColliderComponent >( newEntity, entity );
        CopyComponentIfExists< CharacterControllerComponent >( newEntity, entity );
        CopyComponentIfExists< SpriteRendererComponent >( newEntity, entity );
    }

    Entity Scene::GetEntityByUUID( UUID uuid )
    {
        SM_ASSERT( m_EntityMap.find( uuid ) != m_EntityMap.end(), "Scene::GetEntityByUUID > Invalid UUID" )
        return Entity{ m_EntityMap.at( uuid ), this };
    }

    template < typename ComponentType >
    void Scene::OnComponentAdded( Entity entity, ComponentType &component )
    {
        static_assert( sizeof( ComponentType ) == 0 );
    }

    template <>
    void Scene::OnComponentAdded< ecs::Relationship >( Entity entity, ecs::Relationship &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< IDComponent >( Entity entity, IDComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< TagComponent >( Entity entity, TagComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< TransformComponent >( Entity entity, TransformComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< CameraComponent >( Entity entity, CameraComponent &component )
    {
        component.Camera.SetViewportSize( m_ViewportWidth, m_ViewportHeight );
    }

    template <>
    void Scene::OnComponentAdded< ScriptComponent >( Entity entity, ScriptComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< MeshRendererComponent >( Entity entity, MeshRendererComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< SkinnedMeshRendererComponent >( Entity entity,
        SkinnedMeshRendererComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< AnimatorComponent >( Entity entity, AnimatorComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< RigidbodyComponent >( Entity entity, RigidbodyComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< BoxColliderComponent >( Entity entity, BoxColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< SphereColliderComponent >( Entity entity, SphereColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< CapsuleColliderComponent >( Entity entity, CapsuleColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< CharacterControllerComponent >( Entity entity,
        CharacterControllerComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< SpriteRendererComponent >( Entity entity, SpriteRendererComponent &component )
    {
    }
}
