/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::physics
{
    struct PhysicsMaterial final
    {
        float StaticFriction = 0.3f;
        float DynamicFriction = 0.3f;
        float Restitution = 0.3f;
    };
}