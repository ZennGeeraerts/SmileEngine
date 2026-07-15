/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "project_serializer.h"

#include "smile/core/fs/file.h"
#include "smile/core/yaml/string.h"

namespace smile::project
{
    ProjectSerializer::ProjectSerializer( Ref< Project > pProject ) : m_pContext{ pProject }
    {
    }

    bool ProjectSerializer::Serialize( const fs::Path &filePath )
    {
        const auto &config = m_pContext->GetConfig();

        yaml::Emitter output{};
        {
            output << YAML::BeginMap;
            output << YAML::Key << "Project" << YAML::Value;

            {
                output << YAML::BeginMap;
                output << YAML::Key << "Name" << YAML::Value << config.Name;
                output << YAML::Key << "StartWorld" << YAML::Value << static_cast< Uint64 >( config.StartWorld );
                output << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory;
                output << YAML::Key << "AssetRegistryPath" << YAML::Value << config.AssetRegistryPath;
                output << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath;
                output << YAML::EndMap;
            }

            output << YAML::EndMap;
        }

        fs::File fileOutput{ filePath };
        if ( !fileOutput.OpenOutput( stream::OpeningModeFlags{} ) )
        {
            SM_LOG_ERROR( "Failed to open output file: {}", filePath );
            return false;
        }

        fileOutput.WriteText( output.c_str() );

        fileOutput.Close();

        return true;
    }

    bool ProjectSerializer::Deserialize( const fs::Path &filePath )
    {
        auto &config = m_pContext->GetConfig();

        YAML::Node data;
        try
        {
            data = YAML::LoadFile( filePath.GetData() );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_CRITICALERROR( "Failed to load project file: {0}\n {1}", filePath, e.what() );
            return false;
        }

        YAML::Node projectNode = data["Project"];
        if ( !projectNode )
            return false;

        config.Name = projectNode["Name"].as< std::string >();
        config.StartWorld = projectNode["StartWorld"].as< Uint64 >();
        config.AssetDirectory = projectNode["AssetDirectory"].as< fs::Path >();
        if ( projectNode["AssetRegistryPath"] )
            config.AssetRegistryPath = projectNode["AssetRegistryPath"].as< fs::Path >();
        config.ScriptModulePath = projectNode["ScriptModulePath"].as< fs::Path >();

        return true;
    }
}