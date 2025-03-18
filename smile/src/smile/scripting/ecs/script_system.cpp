/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_system.h"

#include "smile/scripting/script_engine.h"
#include "script_component.h"

#include "smile/core/world/world_manager.h"

namespace smile::scripting::ecs
{
    void ScriptSystem::OnAdd( smile::ecs::ECSEngine &ecsEngine )
    {
        System::OnAdd( ecsEngine );

        ScriptEngine::OnRuntimeStart( world::WorldManager::GetActive().get() );

        // Instantiate all script entities
        auto view = ecsEngine.GetView< ScriptComponent >();
        for ( auto e : view )
        {
            world::Entity entity = { e, world::WorldManager::GetActive().get() };
            ScriptEngine::OnCreateEntity( entity );
        }
    }

    void ScriptSystem::OnRemove( smile::ecs::ECSEngine &ecsEngine )
    {
        ScriptEngine::OnRuntimeStop();
        System::OnRemove( ecsEngine );
    }

    void ScriptSystem::OnUpdate( primitive::Timestep deltaTime )
    {
        auto view = m_pECSEngine->GetView< ScriptComponent >();
        for ( auto e : view )
        {
            world::Entity entity = { e, world::WorldManager::GetActive().get() };
            ScriptEngine::OnUpdateEntity( entity, deltaTime );
        }
    }
}