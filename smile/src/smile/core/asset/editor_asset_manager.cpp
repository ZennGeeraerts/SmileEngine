/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "editor_asset_manager.h"

#include "asset_importer.h"
#include "smile/core/project/project_manager.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace smile::asset
{
    YAML::Emitter &operator<<( YAML::Emitter &out, const std::string_view &strView )
    {
        out << std::string{ strView.begin(), strView.end() };
        return out;
    }

    memory::Ref< Asset > EditorAssetManager::GetAsset( AssetHandle handle )
    {
        // 1. Check if handle is valid
        if ( !IsAssetHandleValid( handle ) )
            return nullptr;

        // 2. Check if asset needs load (and if so, load)
        memory::Ref< Asset > pAsset;

        if ( IsAssetLoaded( handle ) )
        {
            pAsset = m_LoadedAssets.at( handle );
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

            m_LoadedAssets[handle] = pAsset;
        }

        return pAsset;
    }

    bool EditorAssetManager::IsAssetHandleValid( AssetHandle handle ) const
    {
        return ( handle != 0 ) && ( m_AssetRegistry.find( handle ) != m_AssetRegistry.end() );
    }

    bool EditorAssetManager::IsAssetLoaded( AssetHandle handle ) const
    {
        return m_LoadedAssets.find( handle ) != m_LoadedAssets.end();
    }

    void EditorAssetManager::ImportAsset( const std::filesystem::path &path )
    {
        AssetHandle handle{};

        AssetMetadata metadata{};
        metadata.FilePath = path;
        metadata.Type = AssetImporter::GetInstance().GetAssetTypeFromFileExtension( path.extension() );
        SM_ASSERT(
            metadata.Type.IsValid(), "AssetImporter::ImportAsset > Failed to get asset type from file extension" );

        memory::Ref< Asset > pAsset = AssetImporter::GetInstance().ImportAsset( handle, metadata );
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
        auto it = m_AssetRegistry.find( handle );

        if ( it == m_AssetRegistry.end() )
            return nullMetadata;

        return it->second;
    }

    const std::filesystem::path &EditorAssetManager::GetFilePath( AssetHandle handle ) const
    {
        return GetMetadata( handle ).FilePath;
    }

    void EditorAssetManager::SerializeAssetRegistry()
    {
        std::filesystem::path path = project::ProjectManager::GetActive()->GetAssetRegistryPath();

        YAML::Emitter out;
        {
            out << YAML::BeginMap;
            out << YAML::Key << "AssetRegistry" << YAML::Value;

            out << YAML::BeginSeq;
            for ( const auto &[handle, metadata] : m_AssetRegistry )
            {
                out << YAML::BeginMap;
                out << YAML::Key << "Handle" << YAML::Value << handle;
                out << YAML::Key << "FilePath" << YAML::Value << metadata.FilePath.generic_string();
                out << YAML::Key << "Type" << YAML::Value << metadata.Type.GetName();
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
            out << YAML::EndMap;
        }

        std::ofstream fileOutput{ path };
        fileOutput << out.c_str();
    }

    bool EditorAssetManager::DeserializeAssetRegistry()
    {
        std::filesystem::path path = project::ProjectManager::GetActive()->GetAssetRegistryPath();

        YAML::Node data;
        try
        {
            data = YAML::LoadFile( path.string() );
        }
        catch ( const YAML::ParserException &exc )
        {
            SM_LOG_ERROR( "EditorAssetManager::DeserializeAssetRegistry > Failed to load asset registry from file: "
                          "'{0}'\n  with error: '{1}'",
                path.string(),
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
            metadata.FilePath = node["FilePath"].as< std::string >();
            metadata.Type = AssetType{ node["Type"].as< std::string >() };
        }

        return true;
    }
}