/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene_manager.h"

namespace smile::graphic
{
    void SceneManager::OnNewWorld( smile::ecs::ECSEngine *pECSEngine )
    {
        auto pNewScene = memory::CreateRef< Scene >( m_pWindow );
        m_ECSEngineToSceneMap.emplace( pECSEngine, std::move( pNewScene ) );
    }

    void SceneManager::OnActiveWorldChanged( smile::ecs::ECSEngine *pECSEngine )
    {
        auto it = m_ECSEngineToSceneMap.find( pECSEngine );

        SM_ASSERT( it != m_ECSEngineToSceneMap.end(), "SceneManager::OnActiveWorldChanged > World not found in map" );

        m_pActiveScene = it->second;
    }
}