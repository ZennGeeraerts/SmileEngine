/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::physics::ecs
{
    struct ForceComponent final
    {
        ForceComponent() = default;
        ForceComponent( const DirectX::XMFLOAT3 &force, bool autoAwake ) : Force{ force }, AutoAwake{ autoAwake }
        {
        }
        ForceComponent( const ForceComponent & ) = default;

        DirectX::XMFLOAT3 Force{};
        bool AutoAwake = true;
    };
}