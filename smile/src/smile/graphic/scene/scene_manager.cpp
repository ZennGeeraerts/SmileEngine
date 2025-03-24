#include "smpch.h"
#include "scene_manager.h"

namespace smile::graphic
{
    void SceneManager::AddListener( Listener *pListener )
    {
        m_pListeners.push_back( pListener );
    }

    void SceneManager::OnNewWorld( const Ref< world::World > &pWorld )
    {
        auto pNewScene = memory::CreateRef< Scene >( m_pWindow );
        m_WorldToSceneMap.emplace( pWorld->GetUUID(), std::move( pNewScene ) );
    }

    void SceneManager::OnActiveWorldChanged( const Ref< world::World > &pWorld )
    {
        auto it = m_WorldToSceneMap.find( pWorld->GetUUID() );

        SM_ASSERT( it != m_WorldToSceneMap.end(), "SceneManager::OnActiveWorldChanged > World not found in map" );

        m_pActiveScene = it->second;

        for ( Listener *pListener : m_pListeners )
            pListener->OnActiveSceneChanged( m_pActiveScene );
    }
}