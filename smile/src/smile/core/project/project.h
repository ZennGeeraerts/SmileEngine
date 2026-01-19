/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/asset/runtime_asset_manager.h"
#include "smile/core/asset/editor_asset_manager.h"
#include "smile/core/fs/path.h"

namespace smile::project
{
    struct ProjectConfig final
    {
        primitive::String Name = "Untitled";

        asset::AssetHandle StartWorld;

        fs::Path AssetDirectory;
        fs::Path AssetRegistryPath;
        fs::Path ScriptModulePath;
    };

    class Project final
    {
      public:
        const fs::Path &GetProjectDirectory() const
        {
            return m_ProjectDirectory;
        }

        fs::Path GetAssetDirectory() const
        {
            return m_ProjectDirectory / m_Config.AssetDirectory;
        }

        fs::Path GetAssetRegistryPath() const
        {
            return GetAssetDirectory() / m_Config.AssetRegistryPath;
        }

        ProjectConfig &GetConfig()
        {
            return m_Config;
        }

        std::shared_ptr< asset::AssetManagerBase > GetAssetManager() const
        {
            return m_AssetManager;
        }

        std::shared_ptr< asset::RuntimeAssetManager > GetRuntimeAssetManager() const
        {
            return std::static_pointer_cast< asset::RuntimeAssetManager >( m_AssetManager );
        }

        std::shared_ptr< asset::EditorAssetManager > GetEditorAssetManager() const
        {
            return std::static_pointer_cast< asset::EditorAssetManager >( m_AssetManager );
        }

      private:
        ProjectConfig m_Config;
        fs::Path m_ProjectDirectory;
        std::shared_ptr< asset::AssetManagerBase > m_AssetManager;

        friend class ProjectManager;
    };
}
