/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "project_serializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace smile::project
{
    ProjectSerializer::ProjectSerializer( Ref< Project > pProject ) : m_pContext{ pProject }
    {
    }

    bool ProjectSerializer::Serialize( const std::filesystem::path &filePath )
    {
        const auto &config = m_pContext->GetConfig();

        YAML::Emitter output{};
        {
            output << YAML::BeginMap;
            output << YAML::Key << "Project" << YAML::Value;

            {
                output << YAML::BeginMap;
                output << YAML::Key << "Name" << YAML::Value << config.Name;
                output << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
                output << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
                output << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
                output << YAML::EndMap;
            }

            output << YAML::EndMap;
        }

        std::ofstream fileOutput{ filePath };
        fileOutput << output.c_str();

        return true;
    }

    bool ProjectSerializer::Deserialize( const std::filesystem::path &filePath )
    {
        auto &config = m_pContext->GetConfig();

        YAML::Node data;
        try
        {
            data = YAML::LoadFile( filePath.string() );
        }
        catch ( YAML::ParserException e )
        {
            SM_LOG_CRITICALERROR( "Failed to load project file: %s\n %s", filePath.c_str(), e.what() );
            return false;
        }

        YAML::Node projectNode = data["Project"];
        if ( !projectNode )
            return false;

        config.Name = projectNode["Name"].as< std::string >();
        config.StartScene = projectNode["StartScene"].as< std::string >();
        config.AssetDirectory = projectNode["AssetDirectory"].as< std::string >();
        config.ScriptModulePath = projectNode["ScriptModulePath"].as< std::string >();

        return true;
    }
}