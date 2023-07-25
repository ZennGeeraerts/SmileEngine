/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <filesystem>

#include "smile_engine/graphic/resource/texture.h"

namespace smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel();

        void OnImGuiRender();

      private:
        std::filesystem::path m_BaseDirectory;
        std::filesystem::path m_CurrentDirectory;
        Ref< graphic::Texture2D > m_pDirectoryIcon = nullptr;
        Ref< graphic::Texture2D > m_pFileIcon = nullptr;
    };
}
