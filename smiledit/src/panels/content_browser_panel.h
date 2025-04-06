/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/texture.h"
#include "smile/common/memory/ref.h"
#include "smile/core/project/project.h"

#include <filesystem>

namespace smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel( const Ref< project::Project > &pProject );

        void OnImGuiRender();

      private:
        void RefreshAssetTree();

      private:
        Ref< project::Project > m_pProject;

        std::filesystem::path m_BaseDirectory;
        std::filesystem::path m_CurrentDirectory;
        memory::Ref< graphic::Texture > m_pDirectoryIcon = nullptr;
        memory::Ref< graphic::Texture > m_pFileIcon = nullptr;

        struct TreeNode final
        {
            TreeNode( const std::filesystem::path &path, asset::AssetHandle handle ) : Path{ path }, Handle{ handle }
            {
            }

            std::filesystem::path Path;
            asset::AssetHandle Handle = 0;

            Uint32 Parent = std::numeric_limits< Uint32 >::max();
            std::map< std::filesystem::path, Uint32 > Children;
        };

        std::vector< TreeNode > m_TreeNodes;

        std::map< std::filesystem::path, std::vector< std::filesystem::path > > m_AssetTree;

        enum class Mode
        {
            Asset = 0,
            FileSystem = 1
        };

        Mode m_Mode = Mode::Asset;
    };
}
