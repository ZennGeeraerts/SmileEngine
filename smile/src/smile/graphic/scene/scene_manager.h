/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "scene.h"
#include "smile/core/world/world_manager.h"

namespace smile::graphic
{
    class SceneManager final : public world::WorldManager::Listener
    {
      public:
        void Initialize( const window::Window *pWindow )
        {
            m_pActiveScene = memory::CreateRef< Scene >( pWindow );
        }

        memory::Ref< Scene > GetActive() const
        {
            return m_pActiveScene;
        }

        void OnWorldOpened( smile::ecs::ECSEngine &ecsEngine ) override;
        void OnWorldClosed( smile::ecs::ECSEngine &ecsEngine ) override;

      private:
        memory::Ref< Scene > m_pActiveScene;
    };
}