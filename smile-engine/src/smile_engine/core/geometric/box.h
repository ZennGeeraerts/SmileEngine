/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::geometric
{
    struct Box final
    {
        Box() = default;
        Box( const DirectX::XMFLOAT3 &center, const DirectX::XMFLOAT3 &size ) : Center{ center }, Size{ size }
        {
        }

        DirectX::XMFLOAT3 Center = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
    };
}