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
        class Listener
        {
          public:
            virtual void OnActiveSceneChanged( const memory::Ref< Scene > &pScene )
            {
            }
        };

      public:
        void Initialize( const window::Window *pWindow )
        {
            m_pWindow = pWindow;
        }

        memory::Ref< Scene > GetActive() const
        {
            return m_pActiveScene;
        }

        memory::Ref< Scene > GetScene( primitive::UUID worldID ) const
        {
            SM_ASSERT( m_WorldToSceneMap.find( worldID ) != m_WorldToSceneMap.end(),
                "SceneManager::GetScene > World id not found in world to scene map" );

            return m_WorldToSceneMap[worldID];
        }

        void AddListener( Listener *pListener );

        void OnNewWorld( const Ref< world::World > &pWorld ) override;
        void OnActiveWorldChanged( const Ref< world::World > &pWorld ) override;

      private:
        const window::Window *m_pWindow;
        memory::Ref< Scene > m_pActiveScene;
        mutable std::unordered_map< primitive::UUID, memory::Ref< Scene > > m_WorldToSceneMap;
        std::vector< Listener * > m_pListeners{};
    };
}