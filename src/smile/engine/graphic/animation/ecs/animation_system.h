/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/engine/core/ecs/timed_system.h"
#include "smile/engine/graphic/mesh/skinned_mesh.h"
#include "animator_component.h"

namespace smile::graphic::ecs
{
    class AnimationSystem final : public smile::ecs::TimedSystem< AnimationSystem >
    {
      public:
        AnimationSystem() = default;
        ~AnimationSystem() = default;

        void OnUpdate( primitive::Timestep deltaTime );

      private:
        void OnUpdateAnimation( const Ref< SkinnedMesh > &pSkinnedMesh,
            AnimatorComponent &animator,
            primitive::Timestep deltaTime );
        void CalculateBoneTransform( const Ref< SkinnedMesh > &pSkinnedMesh,
            AnimatorComponent &animator,
            AnimationNode *pNode,
            const DirectX::XMFLOAT4X4 &parentTransform );

      private:
        static const Uint32 s_MaxBoneCount;
    };
}