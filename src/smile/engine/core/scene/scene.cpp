/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene.h"

#include "components.h"
#include "smile/engine/graphic/renderer/render_engine.h"
#include "smile/engine/physics/physics_engine.h"
#include "smile/engine/scripting/script_engine.h"

#include "entity.h"

#include "smile/engine/graphic/renderer/ecs/forward_render_pass.h"
#include "smile/engine/graphic/renderer/ecs/wireframe_render_pass.h"
#include "smile/engine/graphic/renderer/ecs/debug_render_pass.h"
#include "smile/engine/graphic/renderer/ecs/render_pass_2d.h"

#include "smile/engine/graphic/camera/ecs/camera_system.h"

#include "smile/engine/core/ecs/relationship.h"

namespace smile::scene
{
    Scene::Scene() : m_TransformSystem{}, m_PhysicsSystem{}, m_AnimationSystem{}, m_CameraSystem{}
    {
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
        return CreateEntity( primitive::UUID{}, name );
    }

    Entity Scene::CreateEntity( primitive::UUID uuid, const std::string &name )
    {
        Entity entity{ m_ECSEngine.CreateEntity(), this };
        entity.AddComponent< ecs::IDComponent >( uuid );
        entity.AddComponent< ecs::TagComponent >( name );
        entity.AddComponent< ecs::TransformComponent >();

        m_EntityMap[uuid] = entity;

        return entity;
    }

    void Scene::DestroyEntity( Entity entity )
    {
        auto pRelationship = entity.TryGetComponent< smile::ecs::Relationship >();
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
                currentChildHandle = currentChild.GetComponent< smile::ecs::Relationship >().Next;

                DestroyEntity( currentChild );
            }
        }

        m_EntityMap.erase( entity.GetUUID() );
        m_ECSEngine.DestroyEntity( entity );
    }

    void Scene::OnOpen()
    {
        m_ECSEngine.AddSystem( &m_TransformSystem );
        m_ECSEngine.AddSystem( &m_AnimationSystem );
        m_ECSEngine.AddSystem( &m_CameraSystem );

        graphic::RenderEngine::AddRenderPass( new graphic::ecs::ForwardRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::ecs::WireframeRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::ecs::DebugRenderPass{ m_ECSEngine } );
        graphic::RenderEngine::AddRenderPass( new graphic::ecs::RenderPass2D{ m_ECSEngine } );
    }

    void Scene::OnClose()
    {
        m_ECSEngine.RemoveSystem( &m_TransformSystem );
        m_ECSEngine.RemoveSystem( &m_AnimationSystem );
        m_ECSEngine.RemoveSystem( &m_CameraSystem );

        graphic::RenderEngine::ClearRenderPasses();
    }

    void Scene::OnRuntimeStart()
    {
        OnSimulationStart();

        // Scripting
        {
            scripting::ScriptEngine::OnRuntimeStart( this );

            // Instantiate all script entities
            auto view = m_ECSEngine.GetView< scripting::ecs::ScriptComponent >();
            for ( auto e : view )
            {
                Entity entity = { e, this };
                scripting::ScriptEngine::OnCreateEntity( entity );
            }
        }
    }

    void Scene::OnRuntimeStop()
    {
        OnSimulationStop();
        scripting::ScriptEngine::OnRuntimeStop();
    }

    void Scene::OnSimulationStart()
    {
        m_ECSEngine.AddSystem( &m_PhysicsSystem );
    }

    void Scene::OnSimulationStop()
    {
        m_ECSEngine.RemoveSystem( &m_PhysicsSystem );
    }

    void Scene::OnUpdateRuntime( primitive::Timestep deltaTime )
    {
        auto view = m_ECSEngine.GetView< scripting::ecs::ScriptComponent >();
        for ( auto e : view )
        {
            Entity entity = { e, this };
            scripting::ScriptEngine::OnUpdateEntity( entity, deltaTime );
        }

        m_ECSEngine.OnUpdate();
        m_PhysicsSystem.OnDebugRender();
        graphic::RenderEngine::OnRender();
    }

    void Scene::OnUpdateSimulation( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_ECSEngine.OnUpdate();
        m_PhysicsSystem.OnDebugRender();
        graphic::RenderEngine::OnRender( editorCamera );
    }

    void Scene::OnUpdateEditor( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_ECSEngine.OnUpdate();
        m_PhysicsSystem.OnDebugRender();
        graphic::RenderEngine::OnRender( editorCamera );
    }

    void Scene::OnViewportResize( Uint32 width, Uint32 height )
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_ECSEngine.GetView< graphic::ecs::CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = m_ECSEngine.GetComponent< graphic::ecs::CameraComponent >( entity );
            if ( !cameraComponent.HasFixedAspectRatio )
            {
                cameraComponent.Camera.SetViewportSize( width, height );
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = m_ECSEngine.GetView< graphic::ecs::CameraComponent >();
        for ( auto entity : view )
        {
            auto &cameraComponent = m_ECSEngine.GetComponent< graphic::ecs::CameraComponent >( entity );
            if ( cameraComponent.IsPrimary )
                return Entity{ entity, this };
        }
        return Entity{};
    }

    template < typename... ComponentType >
    static void CopyComponent( smile::ecs::ECSEngine &dst,
        smile::ecs::ECSEngine &src,
        const std::unordered_map< primitive::UUID, smile::ecs::EntityHandleType > &entityHandleMap )
    {
        (
            [&]()
            {
                auto view = src.GetView< ComponentType >();
                for ( auto entity : view )
                {
                    primitive::UUID uuid = src.GetComponent< ecs::IDComponent >( entity ).ID;
                    SM_ASSERT( entityHandleMap.find( uuid ) != entityHandleMap.end(),
                        "Scene::CopyComponent > uuid not found in enttMap" );
                    smile::ecs::EntityHandleType dstHandleID = entityHandleMap.at( uuid );

                    auto &component = src.GetComponent< ComponentType >( entity );
                    dst.AddOrReplaceComponent< ComponentType >( dstHandleID, component );
                }
            }(),
            ... );
    }

    template < typename... ComponentType >
    static void CopyComponent( ComponentGroup< ComponentType... >,
        smile::ecs::ECSEngine &dst,
        smile::ecs::ECSEngine &src,
        const std::unordered_map< primitive::UUID, smile::ecs::EntityHandleType > &entityHandleMap )
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

        std::unordered_map< primitive::UUID, smile::ecs::EntityHandleType > entityMap{};

        auto &srcSceneEngine = scene->m_ECSEngine;
        auto &dstSceneEngine = pNewScene->m_ECSEngine;
        auto idView = srcSceneEngine.GetView< ecs::IDComponent >();
        for ( auto entity : idView )
        {
            auto uuid = srcSceneEngine.GetComponent< ecs::IDComponent >( entity ).ID;
            const auto &name = srcSceneEngine.GetComponent< ecs::TagComponent >( entity ).Tag;
            Entity newEntity = pNewScene->CreateEntity( uuid, name );
            entityMap[uuid] = static_cast< smile::ecs::EntityHandleType >( newEntity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< smile::ecs::Relationship >( dstSceneEngine, srcSceneEngine, entityMap );
        CopyComponent( AllComponents{}, dstSceneEngine, srcSceneEngine, entityMap );

        return pNewScene;
    }

    void Scene::DuplicateEntity( Entity entity )
    {
        Entity newEntity = CreateEntity( entity.GetName() );

        CopyComponentIfExists< smile::ecs::Relationship >( newEntity, entity );
        CopyComponentIfExists< ecs::TransformComponent >( newEntity, entity );
        CopyComponentIfExists< graphic::ecs::MeshRendererComponent >( newEntity, entity );
        CopyComponentIfExists< graphic::ecs::SkinnedMeshRendererComponent >( newEntity, entity );
        CopyComponentIfExists< graphic::ecs::SpriteRendererComponent >( newEntity, entity );
        CopyComponentIfExists< graphic::ecs::AnimatorComponent >( newEntity, entity );
        CopyComponentIfExists< graphic::ecs::CameraComponent >( newEntity, entity );
        CopyComponentIfExists< scripting::ecs::ScriptComponent >( newEntity, entity );
        CopyComponentIfExists< physics::ecs::RigidbodyComponent >( newEntity, entity );
        CopyComponentIfExists< physics::ecs::BoxColliderComponent >( newEntity, entity );
        CopyComponentIfExists< physics::ecs::SphereColliderComponent >( newEntity, entity );
        CopyComponentIfExists< physics::ecs::CapsuleColliderComponent >( newEntity, entity );
        CopyComponentIfExists< physics::ecs::CharacterControllerComponent >( newEntity, entity );
    }

    Entity Scene::GetEntityByUUID( primitive::UUID uuid )
    {
        SM_ASSERT( m_EntityMap.find( uuid ) != m_EntityMap.end(), "Scene::GetEntityByUUID > Invalid UUID" )
        return Entity{ m_EntityMap.at( uuid ), this };
    }

    void Scene::AddForce( primitive::UUID entityID, const DirectX::XMFLOAT3 &force, bool autoAwake )
    {
        Entity entity = GetEntityByUUID( entityID );
        Ref< physics::Rigidbody > pRigidbody = m_PhysicsSystem.GetRigidbody( entityID );
        pRigidbody->AddForce( force, autoAwake );
    }

    void
    Scene::MoveCharacterController( primitive::UUID entityID, const DirectX::XMFLOAT3 &displacement, float minDist )
    {
        Entity entity = GetEntityByUUID( entityID );
        Ref< physics::CharacterController > pCharacterController = m_PhysicsSystem.GetCharacterController( entityID );
        auto &characterControllerComponent = entity.GetComponent< physics::ecs::CharacterControllerComponent >();
        characterControllerComponent.CollisionFlags = pCharacterController->Move( displacement, minDist );
    }

    template < typename ComponentType >
    void Scene::OnComponentAdded( Entity entity, ComponentType &component )
    {
        static_assert( sizeof( ComponentType ) == 0 );
    }

    template <>
    void Scene::OnComponentAdded< smile::ecs::Relationship >( Entity entity, smile::ecs::Relationship &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< ecs::IDComponent >( Entity entity, ecs::IDComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< ecs::TagComponent >( Entity entity, ecs::TagComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< ecs::TransformComponent >( Entity entity, ecs::TransformComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< graphic::ecs::CameraComponent >( Entity entity,
        graphic::ecs::CameraComponent &component )
    {
        component.Camera.SetViewportSize( m_ViewportWidth, m_ViewportHeight );
    }

    template <>
    void Scene::OnComponentAdded< scripting::ecs::ScriptComponent >( Entity entity,
        scripting::ecs::ScriptComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< graphic::ecs::MeshRendererComponent >( Entity entity,
        graphic::ecs::MeshRendererComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< graphic::ecs::SkinnedMeshRendererComponent >( Entity entity,
        graphic::ecs::SkinnedMeshRendererComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< graphic::ecs::AnimatorComponent >( Entity entity,
        graphic::ecs::AnimatorComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< physics::ecs::RigidbodyComponent >( Entity entity,
        physics::ecs::RigidbodyComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< physics::ecs::BoxColliderComponent >( Entity entity,
        physics::ecs::BoxColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< physics::ecs::SphereColliderComponent >( Entity entity,
        physics::ecs::SphereColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< physics::ecs::CapsuleColliderComponent >( Entity entity,
        physics::ecs::CapsuleColliderComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< physics::ecs::CharacterControllerComponent >( Entity entity,
        physics::ecs::CharacterControllerComponent &component )
    {
    }

    template <>
    void Scene::OnComponentAdded< graphic::ecs::SpriteRendererComponent >( Entity entity,
        graphic::ecs::SpriteRendererComponent &component )
    {
    }
}