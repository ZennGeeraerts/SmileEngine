/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world.h"

#include "components.h"
#include "smile/graphic/renderer/render_engine.h"
#include "smile/physics/physics_engine.h"
#include "smile/scripting/ecs/script_system.h"

#include "entity.h"

#include "smile/graphic/scene/ecs/forward_render_pass.h"
#include "smile/graphic/scene/ecs/wireframe_render_pass.h"
#include "smile/graphic/scene/ecs/debug_render_pass.h"
#include "smile/graphic/scene/ecs/render_pass_2d.h"
#include "smile/graphic/scene/ecs/physics_render_pass.h"

#include "smile/graphic/camera/ecs/camera_system.h"

#include "smile/core/ecs/relationship.h"
#include "smile/core/ecs/state/system_factory.h"

#include "ecs/transform_system.h"
#include "smile/physics/ecs/physics_system.h"
#include "smile/graphic/animation/ecs/animation_system.h"
#include "smile/graphic/camera/ecs/camera_system.h"
#include "smile/graphic/ecs/graphic_system.h"

namespace smile::world
{
    World::World()
    {
        smile::ecs::state::SystemFactory::RegisterSystem< scripting::ecs::ScriptSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< ecs::TransformSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< physics::ecs::PhysicsSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::AnimationSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::CameraSystem >();
        smile::ecs::state::SystemFactory::RegisterSystem< graphic::ecs::GraphicSystem >();

        auto pEditorState = memory::CreateRef< smile::ecs::state::State >();
        pEditorState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pEditorState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
        m_StateManager.AddState( "editor", pEditorState );

        auto pSimulateState = memory::CreateRef< smile::ecs::state::State >();
        pSimulateState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pSimulateState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pSimulateState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
        m_StateManager.AddState( "simulate", pSimulateState );

        auto pRuntimeState = memory::CreateRef< smile::ecs::state::State >();
        pRuntimeState->AddSystem( std::string{ scripting::ecs::ScriptSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ physics::ecs::PhysicsSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::AnimationSystem::GetStaticName() } );
        pRuntimeState->AddSystem( std::string{ graphic::ecs::CameraSystem::GetStaticName() } );
        pRuntimeState->AddOverlaySystem( std::string{ graphic::ecs::GraphicSystem::GetStaticName() } );
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
        m_StateManager.ChangeState( "editor" );

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
        m_StateManager.ChangeState( "runtime" );
    }

    void World::OnRuntimeStop()
    {
        m_StateManager.ChangeState( "editor" );
    }

    void World::OnSimulationStart()
    {
        m_StateManager.ChangeState( "simulate" );
    }

    void World::OnSimulationStop()
    {
        m_StateManager.ChangeState( "editor" );
    }

    void World::OnUpdateRuntime( primitive::Timestep deltaTime )
    {
        m_ECSEngine.OnUpdate();
    }

    void World::OnUpdateSimulation( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        graphic::RenderEngine::GetScene()->SetFallbackCameraData( { &editorCamera, editorCamera.GetTransform() } );
        m_ECSEngine.OnUpdate();
    }

    void World::OnUpdateEditor( primitive::Timestep deltaTime, graphic::EditorCamera &editorCamera )
    {
        graphic::RenderEngine::GetScene()->SetFallbackCameraData( { &editorCamera, editorCamera.GetTransform() } );
        m_ECSEngine.OnUpdate();
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
}