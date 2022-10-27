#pragma once

#include <filesystem>

#include "smile_engine/graphic/resource/texture.h"

namespace smile
{
    class ContentBrowserPanel final
    {
      public:
        ContentBrowserPanel();

        void onImGuiRender();

      private:
        std::filesystem::path currentDirectory;
        Ref< graphic::Texture2D > directoryIcon = nullptr;
        Ref< graphic::Texture2D > fileIcon = nullptr;
    };
}
