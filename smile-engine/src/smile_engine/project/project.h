/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/core/core.h"

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
        static const std::filesystem::path &GetAssetDirectory()
        {
            SM_ASSERT( s_pActiveProject, "Project::GetAssetDirectory > No active project" );
            return s_pActiveProject->m_Config.AssetDirectory;
        }

        static ProjectConfig &GetConfig()
        {
            SM_ASSERT( s_pActiveProject, "Project::GetConfig > No active project" );
            return s_pActiveProject->m_Config;
        }

        static Ref< Project > New();
        static Ref< Project > Load( const std::filesystem::path &path );
        static bool SaveActive( const std::filesystem::path &path );

      private:
        ProjectConfig m_Config;

        inline static Ref< Project > s_pActiveProject;
    };
}