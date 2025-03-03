/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world.h"

#include "components.h"
#include "engine/graphic/renderer/render_engine.h"
#include "engine/physics/physics_engine.h"
#include "engine/scripting/script_engine.h"

#include "entity.h"

#include "engine/graphic/scene/ecs/forward_render_pass.h"
#include "engine/graphic/scene/ecs/wireframe_render_pass.h"
#include "engine/graphic/scene/ecs/debug_render_pass.h"
#include "engine/graphic/scene/ecs/render_pass_2d.h"
#include "engine/graphic/scene/ecs/physics_render_pass.h"

#include "engine/graphic/camera/ecs/camera_system.h"

#include "engine/core/ecs/relationship.h"
#include "ecs/state/system_factory.h"

#include "ecs/transform_system.h"
#include "engine/physics/ecs/physics_system.h"
#include "engine/graphic/animation/ecs/animation_system.h"
#include "engine/graphic/camera/ecs/camera_system.h"
#include "engine/graphic/ecs/graphic_system.h"

namespace smile::world
{
    World::World()
    {
        smile::ecs::state::SystemFactory::RegisterSystem< ecs::TransformSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< physics::ecs::PhysicsSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::AnimationSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::CameraSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::GraphicSystem >();

        auto pEditorState = memory::CreateRef< smile::ecs::state::State >();
        pEditorState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pEditorState->AddSystem( std::string{ graphic::ecs::CameraSystem::GetStaticName() } );
        pEditorState->AddSystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
        m_StateManager.AddState( "editor", pEditorState );

        auto pSimulateState = memory::CreateRef< smile::ecs::state::State >();
        pSimulateState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pSimulateState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pSimulateState->AddSystem( std::string{ graphic::ecs::CameraSystem::GetStaticName() } );
        pSimulateState->AddSystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
        m_StateManager.AddState( "simulate", pSimulateState );

        auto pRuntimeState = memory::CreateRef< smile::ecs::state::State >();
        pRuntimeState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::AnimationSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::CameraSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
        m_StateManager.AddState( "runtime", pRuntimeState );

        m_StateManager.Initialize( &m_ECSEngine, "editor" );
    }

    World::~World()
    {
        m_ECSEngine.Clear();
    }

    Entity World::CreateEntity()
    {
        return CreateEntity( "Entity" );
    }

    Entity World::CreateEntity( const std::string &name )
    {
        return CreateEntity( primitive::UUID{}, name );
    }

    Entity World::CreateEntity( primitive::UUID uuid, const std::string &name )
    {
        Entity entity{ m_ECSEngine.CreateEntity(), this };
        entity.AddComponent< ecs::IDComponent >( uuid );
        entity.AddComponent< ecs::TagComponent >( name );
        entity.AddComponent< ecs::TransformComponent >();

        m_EntityMap[uuid] = entity;

        return entity;
    }

    void World::DestroyEntity( Entity entity )
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

    void World::OnOpen()
    {
        m_StateManager.ChangeState( "editor", { std::string{ graphic::ecs::GraphicSystem::GetStaticName() } } );

        graphic::RenderEngine::GetScene()->AddRenderPass(
            memory::CreateRef< graphic::ecs::ForwardRenderPass >( m_ECSEngine ) );
        graphic::RenderEngine::GetScene()->AddRenderPass(
            memory::CreateRef< graphic::ecs::WireframeRenderPass >( m_ECSEngine ) );
        graphic::RenderEngine::GetScene()->AddRenderPass(
            memory::CreateRef< graphic::ecs::DebugRenderPass >( m_ECSEngine ) );
        graphic::RenderEngine::GetScene()->AddRenderPass(
            memory::CreateRef< graphic::ecs::RenderPass2D >( m_ECSEngine ) );
        graphic::RenderEngine::GetScene()->AddRenderPass(
            memory::CreateRef< graphic::ecs::PhysicsRenderPass >( m_ECSEngine ) );
    }

    void World::OnClose()
    {
        graphic::RenderEngine::GetScene()->ClearRenderPasses();
    }

    void World::OnRuntimeStart()
    {
        m_StateManager.ChangeState( "runtime", { std::string{ graphic::ecs::GraphicSystem::GetStaticName() } } );

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

    void World::OnRuntimeStop()
    {
        m_StateManager.ChangeState( "editor", { std::string{ graphic::ecs::GraphicSystem::GetStaticName() } } );
        scripting::ScriptEngine::OnRuntimeStop();
    }

    void World::OnSimulationStart()
    {
        m_StateManager.ChangeState( "simulate", { std::string{ graphic::ecs::GraphicSystem::GetStaticName() } } );
    }

    void World::OnSimulationStop()
    {
        m_StateManager.ChangeState( "editor", { std::string{ graphic::ecs::GraphicSystem::GetStaticName() } } );
    }

    void World::OnUpdateRuntime( primitive::Timestep deltaTime )
    {
        auto view = m_ECSEngine.GetView< scripting::ecs::ScriptComponent >();
        for ( auto e : view )
        {
            Entity entity = { e, this };
            scripting::ScriptEngine::OnUpdateEntity( entity, deltaTime );
        }

        m_ECSEngine.OnUpdate();
    }

    void World::OnUpdateSimulation( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_ECSEngine.OnUpdate();
    }

    void World::OnUpdateEditor( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        m_ECSEngine.OnUpdate();
    }

    void World::OnViewportResize( Uint32 width, Uint32 height )
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

    Entity World::GetPrimaryCameraEntity()
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
        const std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > &entityHandleMap )
    {
        (
            [&]()
            {
                auto view = src.GetView< ComponentType >();
                for ( auto entity : view )
                {
                    primitive::UUID uuid = src.GetComponent< ecs::IDComponent >( entity ).ID;
                    SM_ASSERT( entityHandleMap.find( uuid ) != entityHandleMap.end(),
                        "world::CopyComponent > uuid not found in enttMap" );
                    smile::ecs::EntityHandle dstHandleID = entityHandleMap.at( uuid );

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
        const std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > &entityHandleMap )
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

    Ref< World > World::Copy( const Ref< World > &pWorld )
    {
        Ref< World > pNewWorld = CreateRef< World >();

        pNewWorld->m_ViewportWidth = pWorld->m_ViewportWidth;
        pNewWorld->m_ViewportHeight = pWorld->m_ViewportHeight;

        std::unordered_map< primitive::UUID, smile::ecs::EntityHandle > entityMap{};

        auto &srcWorldEngine = pWorld->m_ECSEngine;
        auto &dstWorldEngine = pNewWorld->m_ECSEngine;
        auto idView = srcWorldEngine.GetView< ecs::IDComponent >();
        for ( auto entity : idView )
        {
            auto uuid = srcWorldEngine.GetComponent< ecs::IDComponent >( entity ).ID;
            const auto &name = srcWorldEngine.GetComponent< ecs::TagComponent >( entity ).Tag;
            Entity newEntity = pNewWorld->CreateEntity( uuid, name );
            entityMap[uuid] = static_cast< smile::ecs::EntityHandle >( newEntity );
        }

        // Copy components except IDComponent and TagComponent
        CopyComponent< smile::ecs::Relationship >( dstWorldEngine, srcWorldEngine, entityMap );
        CopyComponent( AllComponents{}, dstWorldEngine, srcWorldEngine, entityMap );

        return pNewWorld;
    }

    void World::DuplicateEntity( Entity entity )
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

    Entity World::GetEntityByUUID( primitive::UUID uuid )
    {
        SM_ASSERT( m_EntityMap.find( uuid ) != m_EntityMap.end(), "world::GetEntityByUUID > Invalid UUID" )
        return Entity{ m_EntityMap.at( uuid ), this };
    }

    void World::AddForce( primitive::UUID entityID, const DirectX::XMFLOAT3 &force, bool autoAwake )
    {
        Entity entity = GetEntityByUUID( entityID );

        memory::Ref< physics::ecs::PhysicsSystem > pPhysicsSystem{
            m_StateManager.GetSystem( "smile::physics::ecs::PhysicsSystem" ) };

        Ref< physics::Rigidbody > pRigidbody = pPhysicsSystem->GetRigidbody( entityID );
        pRigidbody->AddForce( force, autoAwake );
    }

    void
    World::MoveCharacterController( primitive::UUID entityID, const DirectX::XMFLOAT3 &displacement, float minDist )
    {
        Entity entity = GetEntityByUUID( entityID );

        memory::Ref< physics::ecs::PhysicsSystem > pPhysicsSystem{
            m_StateManager.GetSystem( "smile::physics::ecs::PhysicsSystem" ) };

        Ref< physics::CharacterController > pCharacterController = pPhysicsSystem->GetCharacterController( entityID );
        auto &characterControllerComponent = entity.GetComponent< physics::ecs::CharacterControllerComponent >();
        characterControllerComponent.CollisionFlags = pCharacterController->Move( displacement, minDist );
    }

    template < typename ComponentType >
    void World::OnComponentAdded( Entity entity, ComponentType &component )
    {
        static_assert( sizeof( ComponentType ) == 0 );
    }

    template <>
    void World::OnComponentAdded< smile::ecs::Relationship >( Entity entity, smile::ecs::Relationship &component )
    {
    }

    template <>
    void World::OnComponentAdded< ecs::IDComponent >( Entity entity, ecs::IDComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< ecs::TagComponent >( Entity entity, ecs::TagComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< ecs::TransformComponent >( Entity entity, ecs::TransformComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< graphic::ecs::CameraComponent >( Entity entity,
        graphic::ecs::CameraComponent &component )
    {
        component.Camera.SetViewportSize( m_ViewportWidth, m_ViewportHeight );
    }

    template <>
    void World::OnComponentAdded< scripting::ecs::ScriptComponent >( Entity entity,
        scripting::ecs::ScriptComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< graphic::ecs::MeshRendererComponent >( Entity entity,
        graphic::ecs::MeshRendererComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< graphic::ecs::SkinnedMeshRendererComponent >( Entity entity,
        graphic::ecs::SkinnedMeshRendererComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< graphic::ecs::AnimatorComponent >( Entity entity,
        graphic::ecs::AnimatorComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< physics::ecs::RigidbodyComponent >( Entity entity,
        physics::ecs::RigidbodyComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< physics::ecs::BoxColliderComponent >( Entity entity,
        physics::ecs::BoxColliderComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< physics::ecs::SphereColliderComponent >( Entity entity,
        physics::ecs::SphereColliderComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< physics::ecs::CapsuleColliderComponent >( Entity entity,
        physics::ecs::CapsuleColliderComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< physics::ecs::CharacterControllerComponent >( Entity entity,
        physics::ecs::CharacterControllerComponent &component )
    {
    }

    template <>
    void World::OnComponentAdded< graphic::ecs::SpriteRendererComponent >( Entity entity,
        graphic::ecs::SpriteRendererComponent &component )
    {
    }
}