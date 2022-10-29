#pragma once

#include <filesystem>

#include "smile_engine/graphic/resource/texture.h"

namespace Smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel();

        void OnImGuiRender();

      private:
        std::filesystem::path m_CurrentDirectory;
        Ref< Graphic::Texture2D > m_pDirectoryIcon = nullptr;
        Ref< Graphic::Texture2D > m_pFileIcon = nullptr;
    };
}
