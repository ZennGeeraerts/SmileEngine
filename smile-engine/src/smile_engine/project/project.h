/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <filesystem>

namespace smile::project
{
    struct ProjectConfig final
    {
        std::string Name = "Untitled";
        std::filesystem::path StartScene;
        std::filesystem::path AssetDirectory;
        std::filesystem::path ScriptModulePath;
    };

    class Project final
    {
      public:
        const std::filesystem::path &GetProjectDirectory() const
        {
            return m_ProjectDirectory;
        }

        std::filesystem::path GetAssetDirectory()
        {
            return m_ProjectDirectory / m_Config.AssetDirectory;
        }

        ProjectConfig &GetConfig()
        {
            return m_Config;
        }

      private:
        ProjectConfig m_Config;
        std::filesystem::path m_ProjectDirectory;

        friend class ProjectManager;
    };
}
