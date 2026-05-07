/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "editor_asset_manager.h"

#include "asset_importer.h"
#include "smile/core/project/project_manager.h"
#include "smile/core/yaml/yaml.h"
#include "smile/core/yaml/string.h"
#include "smile/core/fs/file.h"

namespace smile::asset
{
    memory::Ref< BaseAsset > EditorAssetManager::GetAsset( AssetHandle handle )
    {
        // 1. Check if handle is valid
        if ( !IsAssetHandleValid( handle ) )
            return nullptr;

        // 2. Check if asset needs load (and if so, load)
        memory::Ref< BaseAsset > pAsset;

        if ( IsAssetLoaded( handle ) )
        {
            pAsset = m_LoadedAssets.GetItemAtKey( handle );
        }
        else
        {
            // Load asset
            const AssetMetadata &metadata = GetMetadata( handle );
            pAsset = AssetImporter::GetInstance().ImportAsset( handle, metadata );

            if ( !pAsset )
            {
                SM_LOG_ERROR( "EditorAssetManager::GetAsset > Failed to import asset" );
            }

            pAsset->m_Handle = handle;
            m_LoadedAssets[handle] = pAsset;
        }

        return pAsset;
    }

    bool EditorAssetManager::IsAssetHandleValid( AssetHandle handle ) const
    {
        return ( handle != 0 ) && m_AssetRegistry.HasItemAtKey( handle );
    }

    bool EditorAssetManager::IsAssetLoaded( AssetHandle handle ) const
    {
        return m_LoadedAssets.HasItemAtKey( handle );
    }

    void EditorAssetManager::ImportAsset( const fs::Path &path )
    {
        AssetHandle handle{};

        AssetMetadata metadata{};
        metadata.FilePath = path;
        metadata.Type = AssetImporter::GetInstance().GetAssetTypeFromFileExtension( path.GetExtension() );
        SM_ASSERT_MSG(
            metadata.Type.IsValid(), "AssetImporter::ImportAsset > Failed to get asset type from file extension" );

        memory::Ref< BaseAsset > pAsset = AssetImporter::GetInstance().ImportAsset( handle, metadata );
        if ( pAsset )
        {
            pAsset->m_Handle = handle;
            m_LoadedAssets[handle] = pAsset;
            m_AssetRegistry[handle] = metadata;
            SerializeAssetRegistry();
        }
    }

    const AssetMetadata &EditorAssetManager::GetMetadata( AssetHandle handle ) const
    {
        static AssetMetadata nullMetadata;
        auto it = m_AssetRegistry.FindItem( handle );

        if ( it == m_AssetRegistry.end() )
            return nullMetadata;

        return it.GetItem();
    }

    const fs::Path &EditorAssetManager::GetFilePath( AssetHandle handle ) const
    {
        return GetMetadata( handle ).FilePath;
    }

    void EditorAssetManager::SerializeAssetRegistry()
    {
        fs::Path path = project::ProjectManager::GetActive()->GetAssetRegistryPath();

        YAML::Emitter out;
        {
            out << YAML::BeginMap;
            out << YAML::Key << "AssetRegistry" << YAML::Value;

            out << YAML::BeginSeq;
            for ( const auto &[handle, metadata] : m_AssetRegistry )
            {
                out << YAML::BeginMap;
                out << YAML::Key << "Handle" << YAML::Value << handle;
                out << YAML::Key << "FilePath" << YAML::Value << fs::Path::FromPlatformPath( metadata.FilePath );
                out << YAML::Key << "Type" << YAML::Value << metadata.Type.GetName();
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
            out << YAML::EndMap;
        }

        fs::File fileOutput{ path };
        if ( !fileOutput.OpenOutput( stream::OpeningModeFlags{} ) )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", path );
            return;
        }

        fileOutput.WriteText( out.c_str() );

        fileOutput.Close();
    }

    bool EditorAssetManager::DeserializeAssetRegistry()
    {
        fs::Path path = project::ProjectManager::GetActive()->GetAssetRegistryPath();

        YAML::Node data;
        try
        {
            data = YAML::LoadFile( path.GetData() );
        }
        catch ( const YAML::ParserException &exc )
        {
            SM_LOG_ERROR( "EditorAssetManager::DeserializeAssetRegistry > Failed to load asset registry from file: "
                          "'{0}'\n  with error: '{1}'",
                path,
                exc.what() );

            return false;
        }

        auto rootNode = data["AssetRegistry"];
        if ( !rootNode )
            return false;

        for ( const auto &node : rootNode )
        {
            AssetHandle handle = node["Handle"].as< Uint64 >();

            AssetMetadata &metadata = m_AssetRegistry[handle];
            metadata.FilePath = node["FilePath"].as< fs::Path >();
            metadata.Type = AssetType{ node["Type"].as< primitive::String >() };
        }

        return true;
    }
}