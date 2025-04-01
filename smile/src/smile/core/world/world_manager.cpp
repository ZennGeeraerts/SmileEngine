/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_manager.h"

#include "smile/core/asset/asset_manager.h"

namespace smile::world
{
    memory::Ref< World > WorldManager::New()
    {
        auto pNewWorld = memory::CreateRef< World >();
        Open( pNewWorld );
        return pNewWorld;
    }

    memory::Ref< World > WorldManager::Load( const std::filesystem::path &path )
    {
        memory::Ref< World > pWorld = s_WorldLoader.LoadWorld( path );
        if ( pWorld )
        {
            Open( pWorld );
            return pWorld;
        }
        else
        {
            return nullptr;
        }
    }

    memory::Ref< World > WorldManager::Load( asset::AssetHandle handle )
    {
        memory::Ref< World > pWorld = asset::AssetManager::GetAsset< world::World >( handle );
        if ( pWorld )
        {
            Open( pWorld );
            return pWorld;
        }
        else
        {
            return nullptr;
        }
    }

    void WorldManager::UnloadActive()
    {
        s_pActiveWorld.Reset();
    }

    void WorldManager::Open( memory::Ref< World > pWorld )
    {
        if ( s_pActiveWorld )
        {
            for ( Listener *pListener : s_pListeners )
                pListener->OnWorldClosed( s_pActiveWorld->m_ECSEngine );
        }

        s_pActiveWorld = pWorld;

        for ( Listener *pListener : s_pListeners )
            pListener->OnWorldOpened( pWorld->m_ECSEngine );
    }

    void WorldManager::SaveActive( const std::filesystem::path &path )
    {
        s_WorldLoader.SaveWorld( s_pActiveWorld, path );
    }

    memory::Ref< World > WorldManager::CopyActive()
    {
        auto pNewWorld = World::Copy( s_pActiveWorld );
        return pNewWorld;
    }

    void WorldManager::AddListener( Listener *pListener )
    {
        s_pListeners.push_back( pListener );
    }
}