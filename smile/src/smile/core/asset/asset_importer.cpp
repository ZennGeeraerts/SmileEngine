/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "asset_importer.h"

#include "asset_loader.h"

namespace smile::asset
{
    void AssetImporter::RegisterLoader( BaseAssetLoader *pLoader )
    {
        const AssetType assetType = pLoader->GetType();

        SM_ASSERT_MSG( !m_AssetLoaderMap.HasItemAtKey( assetType ), "Asset type collision" );
        m_AssetLoaderMap.Add( assetType, pLoader );

        for ( const fs::Path &extension : pLoader->GetExtensions() )
        {
            SM_ASSERT_MSG( !m_AssetExtensionMap.HasItemAtKey( extension ), "Extension collision" );
            m_AssetExtensionMap.Add( extension, assetType );
        }
    }

    memory::Ref< Asset > AssetImporter::ImportAsset( AssetHandle handle, const AssetMetadata &metadata )
    {
        if ( !m_AssetLoaderMap.HasItemAtKey( metadata.Type ) )
        {
            SM_LOG_ERROR(
                "AssetImporter::ImportAsset > No loader registered for asset type: {}", metadata.Type.GetName() );

            return nullptr;
        }

        return m_AssetLoaderMap[metadata.Type]->Load( handle, metadata );
    }

    AssetType AssetImporter::GetAssetTypeFromFileExtension( const fs::Path &extension )
    {
        if ( !m_AssetExtensionMap.HasItemAtKey( extension ) )
        {
            SM_LOG_WARNING( "GetAssetTypeFromFileExtension > Could not find AssetType for {}", extension );
            return AssetType::NullType();
        }

        return m_AssetExtensionMap[extension];
    }
}