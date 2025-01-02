/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "animation_system.h"

#include "smile/engine/core/ecs/ecs_engine.h"
#include "smile/engine/graphic/mesh/ecs/skinned_mesh_renderer_component.h"
#include "smile/engine/graphic/animation/ecs/animator_component.h"
#include "smile/engine/core/scene/ecs/transform_component.h"

namespace smile::graphic::ecs
{
    const Uint32 AnimationSystem::s_MaxBoneCount{ 60 };

    void AnimationSystem::OnUpdate( primitive::Timestep deltaTime )
    {
        auto group = m_pECSEngine->GetGroup< SkinnedMeshRendererComponent, AnimatorComponent >(
            smile::ecs::g_Get< scene::ecs::TransformComponent > );
        for ( auto entity : group )
        {
            const auto &[mesh, animator, transform] =
                m_pECSEngine
                    ->GetComponents< SkinnedMeshRendererComponent, AnimatorComponent, scene::ecs::TransformComponent >(
                        entity );

            animator.Transforms.resize( s_MaxBoneCount );
            OnUpdateAnimation( mesh.pSkinnedMesh, animator, deltaTime );

            if ( animator.IsPlaying )
                mesh.pMaterial->GetShader()->UploadMat4Array( "Bones", animator.Transforms );
        }
    }

    void AnimationSystem::OnUpdateAnimation( const Ref< SkinnedMesh > &pSkinnedMesh,
        AnimatorComponent &animator,
        primitive::Timestep deltaTime )
    {
        if ( !animator.IsPlaying )
            return;

        Ref< AnimationClip > pCurrentClip = animator.pAnimationClips[animator.CurrentClipIndex];
        if ( !pCurrentClip )
            return;

        float passedTicks = deltaTime * pCurrentClip->TicksPerSecond;
        passedTicks = fmod( passedTicks, pCurrentClip->Duration );

        if ( !animator.IsReversed )
        {
            animator.TickCount += passedTicks;
            if ( animator.TickCount > pCurrentClip->Duration )
                animator.TickCount = 0;
        }
        else
        {
            animator.TickCount -= passedTicks;
            if ( animator.TickCount < 0 )
                animator.TickCount = pCurrentClip->Duration;
        }

        DirectX::XMFLOAT4X4 identityMatrix{};
        DirectX::XMStoreFloat4x4( &identityMatrix, DirectX::XMMatrixIdentity() );
        CalculateBoneTransform( pSkinnedMesh, animator, &pCurrentClip->RootNode, identityMatrix );
    }

    void AnimationSystem::CalculateBoneTransform( const Ref< SkinnedMesh > &pSkinnedMesh,
        AnimatorComponent &animator,
        AnimationNode *pNode,
        const DirectX::XMFLOAT4X4 &parentTransform )
    {
        const std::string &nodeName = pNode->Name;
        DirectX::XMFLOAT4X4 nodeTransform = pNode->Transform;
        Ref< AnimationClip > pCurrentClip = animator.pAnimationClips[animator.CurrentClipIndex];

        auto it = std::find_if( pCurrentClip->Bones.begin(),
            pCurrentClip->Bones.end(),
            [nodeName]( const Bone &bone ) { return bone.GetName() == nodeName; } );

        if ( it != pCurrentClip->Bones.end() )
        {
            Bone &bone = ( *it );
            bone.OnUpdate( animator.TickCount );
            nodeTransform = bone.GetLocalTransform();
        }

        DirectX::XMMATRIX globalTransformMat =
            DirectX::XMLoadFloat4x4( &nodeTransform ) * DirectX::XMLoadFloat4x4( &parentTransform );
        DirectX::XMFLOAT4X4 globalTransform{};
        DirectX::XMStoreFloat4x4( &globalTransform, globalTransformMat );

        if ( pSkinnedMesh->SkeletonMap.find( nodeName ) != pSkinnedMesh->SkeletonMap.end() )
        {
            Uint32 id = pSkinnedMesh->SkeletonMap[nodeName].ID;
            SM_ASSERT( id < s_MaxBoneCount, "AnimationSystem::CalculateBoneTransform > Max bone count reached" );

            DirectX::XMMATRIX transformMat =
                DirectX::XMLoadFloat4x4( &pSkinnedMesh->SkeletonMap[nodeName].Offset ) * globalTransformMat;
            DirectX::XMStoreFloat4x4( &animator.Transforms[id], transformMat );
        }

        for ( Uint32 i{}; i < pNode->ChildrenCount; ++i )
            CalculateBoneTransform( pSkinnedMesh, animator, &pNode->Children[i], globalTransform );
    }
}