/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/animation/bone.h"

namespace smile::graphic
{
    struct AnimationNode final
    {
        DirectX::XMFLOAT4X4 Transform{};
        std::string Name{};
        Uint32 ChildrenCount{};
        std::vector< AnimationNode > Children;
    };

    struct AnimationClip final
    {
        std::string Name{};
        float Duration{};
        float TicksPerSecond{};
        std::vector< Bone > Bones{};
        AnimationNode RootNode{};
    };
}