/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/animation/bone.h"

namespace smile::graphic
{
    struct AnimationNode final
    {
        DirectX::XMFLOAT4X4 Transform{};
        primitive::String Name{};
        Count ChildrenCount{};
        primitive::Vector< AnimationNode > Children;
    };

    struct AnimationClip final
    {
        primitive::String Name{};
        float Duration{};
        float TicksPerSecond{};
        primitive::Vector< Bone > Bones{};
        AnimationNode RootNode{};
    };
}