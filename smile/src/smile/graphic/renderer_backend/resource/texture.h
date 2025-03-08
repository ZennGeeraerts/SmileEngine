/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "foundation/compiled.h"
#include "memory/object.h"

#include <string>

namespace smile::graphic
{
    struct Texture : public memory::Object
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
}
