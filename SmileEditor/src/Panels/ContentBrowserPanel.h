#pragma once

#include <filesystem>

#include "SmileEngine/Renderer/Texture.h"

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
