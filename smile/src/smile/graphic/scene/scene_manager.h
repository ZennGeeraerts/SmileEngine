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
            m_pWindow = pWindow;
        }

        memory::Ref< Scene > GetActive() const
        {
            return m_pActiveScene;
        }

        memory::Ref< Scene > GetScene( smile::ecs::ECSEngine *pECSEngine ) const
        {
            SM_ASSERT( m_ECSEngineToSceneMap.find( pECSEngine ) != m_ECSEngineToSceneMap.end(),
                "SceneManager::GetScene > ECS engine not found in world to scene map" );

            return m_ECSEngineToSceneMap[pECSEngine];
        }

        void OnNewWorld( smile::ecs::ECSEngine *pECSEngine ) override;
        void OnActiveWorldChanged( smile::ecs::ECSEngine *pECSEngine ) override;

      private:
        const window::Window *m_pWindow;
        memory::Ref< Scene > m_pActiveScene;
        mutable std::unordered_map< smile::ecs::ECSEngine *, memory::Ref< Scene > > m_ECSEngineToSceneMap;
    };
}