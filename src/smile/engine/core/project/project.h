/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/engine/core/asset/runtime_asset_manager.h"
#include "smile/engine/core/asset/editor_asset_manager.h"

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

        std::shared_ptr< asset::AssetManagerBase > GetAssetManager() const
        {
            return m_AssetManager;
        }
        std::shared_ptr< asset::RuntimeAssetManager > GetRuntimeAssetManager()
        {
            return std::static_pointer_cast< asset::RuntimeAssetManager >( m_AssetManager );
        }

        std::shared_ptr< asset::EditorAssetManager > GetEditorAssetManager()
        {
            return std::static_pointer_cast< asset::EditorAssetManager >( m_AssetManager );
        }

      private:
        ProjectConfig m_Config;
        std::filesystem::path m_ProjectDirectory;
        std::shared_ptr< asset::AssetManagerBase > m_AssetManager;

        friend class ProjectManager;
    };
}
