#pragma once

#include <filesystem>

#include "smile_engine/renderer/resource/texture.h"

namespace smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel();

        void onImGuiRender();

      private:
        std::filesystem::path currentDirectory;
        Ref< renderer::Texture2D > directoryIcon = nullptr;
        Ref< renderer::Texture2D > fileIcon = nullptr;
    };
}
