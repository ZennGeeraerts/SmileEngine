/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/common/compiled/compiled.h"
#include <string>

namespace smile::graphic
{
    struct Texture
    {
        Texture() = default;
        virtual ~Texture() = default;
        Texture( const Texture & ) = delete;
        Texture( Texture && ) = delete;

        virtual void *GetData() const = 0;

        std::string FilePath;
        Uint32 Width = 0;
        Uint32 Height = 0;
    };

    struct Texture2D : public Texture
    {
      public:
        Texture2D() = default;
        virtual ~Texture2D() = default;
    };

    struct TextureCube : public Texture
    {
      public:
        TextureCube() = default;
        virtual ~TextureCube() = default;
    };
}
