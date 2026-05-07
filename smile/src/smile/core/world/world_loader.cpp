/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "world_loader.h"

#include "world_serializer.h"

#include "smile/core/asset/asset_importer.h"
#include "smile/core/project/project_manager.h"

namespace smile::world
{
    WorldLoader::WorldLoader()
    {
        asset::AssetImporter::GetInstance().RegisterLoader( this );
    }

    memory::Ref< asset::BaseAsset > WorldLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return Load( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    World::Ref WorldLoader::Load( const fs::Path &path ) const
    {
        if ( path.IsEmpty() )
        {
            SM_LOG_WARNING( "WorldLoader::Load > Failed to load world: the path was empty" );
            return nullptr;
        }

        if ( path.GetExtension() != ".smile" )
        {
            SM_LOG_WARNING( "WorldLoader::Load > Failed to load world: wrong file extension" );
            return nullptr;
        }

        World::Ref pWorld = memory::CreateRef< World >();

        WorldSerializer worldSerializer{ pWorld };
        if ( !worldSerializer.Deserialize( path ) )
            return nullptr;

        return pWorld;
    }

    void WorldLoader::SaveWorld( memory::Ref< World > pWorld, const fs::Path &path ) const
    {
        WorldSerializer worldSerializer{ pWorld };
        worldSerializer.Serialize( project::ProjectManager::GetAssetFileSystemPath( path ) );
    }
}