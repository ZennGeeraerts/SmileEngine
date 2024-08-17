/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::geometric
{
    struct Circle final
    {
        DirectX::XMFLOAT2 Center{ 0.0f, 0.0f };
        float Radius{ 0.0f };
    };
}