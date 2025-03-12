/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/resource/texture.h"
#include "smile/common/memory/ref.h"

#include <filesystem>

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
        memory::Ref< graphic::Texture > m_pDirectoryIcon = nullptr;
        memory::Ref< graphic::Texture > m_pFileIcon = nullptr;
    };
}
