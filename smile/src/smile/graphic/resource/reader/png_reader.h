/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/ref.h"

#include <filesystem>

namespace smile::graphic
{
    class Image;

    class PNGReader final
    {
      public:
        static memory::Ref< Image > Read( const std::filesystem::path &path );

      private:
        PNGReader() = delete;
    };
}