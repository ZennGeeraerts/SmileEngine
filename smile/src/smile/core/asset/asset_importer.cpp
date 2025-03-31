/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "asset_importer.h"

#include "asset_loader.h"

#include <map>

namespace smile::asset
{
    static std::map< AssetType, AssetLoader * > s_AssetLoaderMap{};
    static std::map< std::filesystem::path, AssetType > s_AssetExtensionMap{};

    void AssetImporter::RegisterLoader( AssetLoader *pLoader )
    {
        const AssetType assetType = pLoader->GetType();

        s_AssetLoaderMap.insert( std::make_pair( assetType, pLoader ) );

        for ( const std::filesystem::path &extension : pLoader->GetExtensions() )
        {
            s_AssetExtensionMap.insert( std::make_pair( extension, assetType ) );
        }
    }

    memory::Ref< Asset > AssetImporter::ImportAsset( AssetHandle handle, const AssetMetadata &metadata )
    {
        if ( s_AssetLoaderMap.find( metadata.Type ) == s_AssetLoaderMap.end() )
        {
            SM_LOG_ERROR( "AssetImporter::ImportAsset > No loader registered for asset type: {}",
                static_cast< Uint16 >( metadata.Type ) );

            return nullptr;
        }

        return s_AssetLoaderMap[metadata.Type]->Load( handle, metadata );
    }

    AssetType AssetImporter::GetAssetTypeFromFileExtension( const std::filesystem::path &extension )
    {
        if ( s_AssetExtensionMap.find( extension ) == s_AssetExtensionMap.end() )
        {
            SM_LOG_WARNING( "GetAssetTypeFromFileExtension > Could not find AssetType for {}", extension.string() );
            return AssetType::None;
        }

        return s_AssetExtensionMap[extension];
    }
}