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

    memory::Ref< asset::Asset > WorldLoader::Load( asset::AssetHandle handle,
        const asset::AssetMetadata &metadata ) const
    {
        return LoadWorld( project::ProjectManager::GetAssetFileSystemPath( metadata.FilePath ) );
    }

    memory::Ref< World > WorldLoader::LoadWorld( const std::filesystem::path &path ) const
    {
        if ( path.empty() )
        {
            SM_LOG_WARNING( "WorldLoader::LoadWorld > Failed to load world: the path was empty" );
            return nullptr;
        }

        if ( path.extension() != ".smile" )
        {
            SM_LOG_WARNING( "WorldLoader::LoadWorld > Failed to load world: wrong file extension" );
            return nullptr;
        }

        memory::Ref< World > pWorld = memory::CreateRef< World >();

        WorldSerializer worldSerializer{ pWorld };
        if ( !worldSerializer.Deserialize( path ) )
            return nullptr;

        return pWorld;
    }

    void WorldLoader::SaveWorld( memory::Ref< World > pWorld, const std::filesystem::path &path ) const
    {
        WorldSerializer worldSerializer{ pWorld };
        worldSerializer.Serialize( project::ProjectManager::GetAssetFileSystemPath( path ) );
    }
}