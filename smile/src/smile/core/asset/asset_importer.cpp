/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "asset_importer.h"

#include "asset_loader.h"

namespace smile::asset
{
    void AssetImporter::RegisterLoader( AssetLoader *pLoader )
    {
        const AssetType assetType = pLoader->GetType();

        SM_ASSERT_MSG( m_AssetLoaderMap.find( assetType ) == m_AssetLoaderMap.end(), "Asset type collision" );
        m_AssetLoaderMap.insert( std::make_pair( assetType, pLoader ) );

        for ( const std::filesystem::path &extension : pLoader->GetExtensions() )
        {
            SM_ASSERT_MSG( m_AssetExtensionMap.find( extension ) == m_AssetExtensionMap.end(), "Extension collision" );
            m_AssetExtensionMap.insert( std::make_pair( extension, assetType ) );
        }
    }

    memory::Ref< Asset > AssetImporter::ImportAsset( AssetHandle handle, const AssetMetadata &metadata )
    {
        if ( m_AssetLoaderMap.find( metadata.Type ) == m_AssetLoaderMap.end() )
        {
            SM_LOG_ERROR(
                "AssetImporter::ImportAsset > No loader registered for asset type: {}", metadata.Type.GetName() );

            return nullptr;
        }

        return m_AssetLoaderMap[metadata.Type]->Load( handle, metadata );
    }

    AssetType AssetImporter::GetAssetTypeFromFileExtension( const std::filesystem::path &extension )
    {
        if ( m_AssetExtensionMap.find( extension ) == m_AssetExtensionMap.end() )
        {
            SM_LOG_WARNING( "GetAssetTypeFromFileExtension > Could not find AssetType for {}", extension.string() );
            return AssetType::NullType();
        }

        return m_AssetExtensionMap[extension];
    }
}