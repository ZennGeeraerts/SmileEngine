/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/ecs/system.h"
#include "smile_engine/scene/components.h"

namespace smile::graphic
{
    class AnimationSystem final : public ecs::System
    {
      public:
        AnimationSystem() = default;
        ~AnimationSystem() = default;

        void OnUpdate( Timestep deltaTime ) override;

      private:
        void OnUpdateAnimation( const Ref< SkinnedMesh > &pSkinnedMesh,
            scene::AnimatorComponent &animator,
            Timestep deltaTime );
        void CalculateBoneTransform( const Ref< SkinnedMesh > &pSkinnedMesh,
            scene::AnimatorComponent &animator,
            AnimationNode *pNode,
            const DirectX::XMFLOAT4X4 &parentTransform );

      private:
        static const Uint32 s_MaxBoneCount;
    };
}