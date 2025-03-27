/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world.h"

#include "entity.h"
#include "smile/core/world/ecs/transform_system.h"
#include "smile/core/ecs/relationship.h"
#include "smile/graphic/renderer/render_engine.h"

namespace smile::world
{
    std::unordered_map< foundation::TypeID, World::CopyComponentFunctions > World::s_CopyComponentFuncs{};

    World::World() : m_UUID{}
    {
        auto pDefaultState = memory::CreateRef< smile::ecs::state::State >();
        pDefaultState->AddSystem( std::string{ ecs::TransformSystem::GetStaticName() } );
        m_StateManager.AddState( "default", pDefaultState );

        m_StateManager.Initialize( &m_ECSEngine, "default" );
    }

    World::~World()
    {
        m_ECSEngine.Clear();
    }

    memory::Ref< smile::ecs::state::State > World::CreateState( const std::string &name )
    {
        auto pState = memory::CreateRef< smile::ecs::state::State >();
        m_StateManager.AddState( name, pState );
        return pState;
    }

    void World::ChangeState( const std::string &name )
    {
        m_StateManager.ChangeState( name );
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

    void World::OnUpdate( primitive::Timestep deltaTime )
    {
        m_ECSEngine.OnUpdate();
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

        for ( const auto &pair : s_CopyComponentFuncs )
        {
            pair.second.ECSEngineCopy( srcWorldEngine, dstWorldEngine, entityMap );
        }

        pNewWorld->m_StateManager =
            smile::ecs::state::StateManager::Copy( pWorld->m_StateManager, &pNewWorld->m_ECSEngine );

        return pNewWorld;
    }

    void World::DuplicateEntity( Entity entity )
    {
        Entity newEntity = CreateEntity( entity.GetName() );

        for ( const auto &pair : s_CopyComponentFuncs )
        {
            pair.second.EntityCopy( entity, newEntity );
        }
    }

    Entity World::GetEntityByUUID( primitive::UUID uuid )
    {
        SM_ASSERT( m_EntityMap.find( uuid ) != m_EntityMap.end(), "world::GetEntityByUUID > Invalid UUID" )
        return Entity{ m_EntityMap.at( uuid ), this };
    }
}