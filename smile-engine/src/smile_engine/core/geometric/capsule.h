/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::geometric
{
    struct Capsule final
    {
        Capsule() = default;
        Capsule( const DirectX::XMFLOAT3 &center, float radius, float height )
            : Center{ center }, Radius{ radius }, Height{ height }
        {
        }

        DirectX::XMFLOAT3 Center{ 0.0f, 0.0f, 0.0f };
        float Radius{ 0.0f };
        float Height{ 0.0f };
    };
}