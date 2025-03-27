/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "scene_manager.h"

namespace smile::graphic
{
    void SceneManager::OnWorldOpened( smile::ecs::ECSEngine &ecsEngine )
    {
        m_pActiveScene->OnAdd( ecsEngine );
    }

    void SceneManager::OnWorldClosed( smile::ecs::ECSEngine &ecsEngine )
    {
        m_pActiveScene->OnRemove( ecsEngine );
    }
}