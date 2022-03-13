#pragma once

#include <filesystem>

#include "smile_engine/renderer/resource/texture.h"

namespace smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel();

        void OnImGuiRender();

      private:
        std::filesystem::path m_CurrentDirectory;
        Ref< Texture2D > m_pDirectoryIcon = nullptr;
        Ref< Texture2D > m_pFileIcon = nullptr;
    };
}
