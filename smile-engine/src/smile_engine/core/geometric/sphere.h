/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::geometric
{
    struct Sphere final
    {
        Sphere( const DirectX::XMFLOAT3 &center, float radius ) : Center{ center }, Radius{ radius }
        {
        }

        DirectX::XMFLOAT3 Center{ 0.0f, 0.0f, 0.0f };
        float Radius{ 0.0f };
    };
}