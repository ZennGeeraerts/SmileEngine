/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_manager.h"

#include "world_serializer.h"

namespace smile::world
{
    Ref< World > WorldManager::New()
    {
        auto pNewWorld = CreateRef< World >();

        for ( Listener *pListener : s_pListeners )
            pListener->OnNewWorld( &pNewWorld->m_ECSEngine );

        Open( pNewWorld );

        return pNewWorld;
    }

    Ref< World > WorldManager::Load( const std::filesystem::path &path )
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "WorldManager::Load > Failed to load world: the path was empty" );
            return nullptr;
        }

        if ( path.extension().string() != ".smile" )
        {
            SM_LOG_WARNING( "WorldManager::Load > Failed to load world: wrong file extention" );
            return nullptr;
        }

        Ref< world::World > pNewWorld = CreateRef< world::World >();
        for ( Listener *pListener : s_pListeners )
            pListener->OnNewWorld( &pNewWorld->m_ECSEngine );

        WorldSerializer worldSerializer{ pNewWorld };
        if ( worldSerializer.Deserialize( path.string() ) )
        {
            Open( pNewWorld );
            return pNewWorld;
        }
        else
        {
            return nullptr;
        }
    }

    void WorldManager::UnloadActive()
    {
        s_pActiveWorld.reset();
    }

    void WorldManager::Open( const Ref< World > &pWorld )
    {
        if ( s_pActiveWorld )
            s_pActiveWorld->OnClose();

        s_pActiveWorld = pWorld;
        s_pActiveWorld->OnOpen();

        for ( Listener *pListener : s_pListeners )
            pListener->OnActiveWorldChanged( &pWorld->m_ECSEngine );
    }

    void WorldManager::SaveActive( const std::filesystem::path &path )
    {
        WorldSerializer worldSerializer{ s_pActiveWorld };
        worldSerializer.Serialize( path.string() );
    }

    Ref< World > WorldManager::CopyActive()
    {
        auto pNewWorld = World::Copy( s_pActiveWorld );

        for ( Listener *pListener : s_pListeners )
            pListener->OnNewWorld( &pNewWorld->m_ECSEngine );

        return pNewWorld;
    }

    void WorldManager::AddListener( Listener *pListener )
    {
        s_pListeners.push_back( pListener );
    }
}