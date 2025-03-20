/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::physics::ecs
{
    struct MoveComponent final
    {
        MoveComponent() = default;
        MoveComponent( const DirectX::XMFLOAT3 &displacement, float minDist )
            : Displacement{ displacement }, MinDist{ minDist }
        {
        }
        MoveComponent( const MoveComponent & ) = default;

        DirectX::XMFLOAT3 Displacement{};
        float MinDist;
    };
}