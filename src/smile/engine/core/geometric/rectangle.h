/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::geometric
{
    struct Rectangle final
    {
        union
        {
            struct
            {
                DirectX::XMFLOAT2 Center;
                DirectX::XMFLOAT2 Extent;
            };
            struct
            {
                float x, y, Width, Height;
            };
        };
    };
}