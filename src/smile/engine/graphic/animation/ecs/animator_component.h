/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/mesh/model.h"

namespace smile::graphic::ecs
{
    struct AnimatorComponent final
    {
        AnimatorComponent() = default;
        AnimatorComponent( const AnimatorComponent & ) = default;

        std::vector< Ref< graphic::AnimationClip > > pAnimationClips;
        Uint32 CurrentClipIndex = 0;

        // For serialization
        Ref< graphic::Model > pModel = nullptr;

        // For internal use
        std::vector< DirectX::XMFLOAT4X4 > Transforms;
        float TickCount = 0.f;
        bool IsPlaying = true;
        bool IsReversed = false;
    };
}