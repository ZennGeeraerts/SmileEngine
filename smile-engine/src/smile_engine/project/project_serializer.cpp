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

        std::ifstream fileInput{ filePath };
        std::stringstream strStream{};
        strStream << fileInput.rdbuf();

        YAML::Node data = YAML::Load( strStream.str() );
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