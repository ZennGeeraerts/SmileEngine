#include "smpch.h"
#include "mesh_animator.h"

namespace smile::renderer
{
    const Uint32 MeshAnimator::maxBoneCount{ 70 };

    MeshAnimator::MeshAnimator( const Ref< SkinnedMeshFilter > &mesh ) : skinnedMesh{ mesh }
    {
        transforms.resize( maxBoneCount );
    }

    void MeshAnimator::onUpdate( Timestep delta_time )
    {
        if ( clipSet && playing )
        {
            float passed_ticks = delta_time * currentClip.ticksPerSecond;
            passed_ticks = fmod( passed_ticks, currentClip.duration );

            if ( !reversed )
            {
                tickCount += passed_ticks;
                if ( tickCount > currentClip.duration )
                    tickCount = 0;
            }
            else
            {
                tickCount -= passed_ticks;
                if ( tickCount < 0 )
                    tickCount = currentClip.duration;
            }

            DirectX::XMFLOAT4X4 identity_matrix{};
            DirectX::XMStoreFloat4x4( &identity_matrix, DirectX::XMMatrixIdentity() );
            calculateBoneTransform( &currentClip.rootNode, identity_matrix );
        }
    }

    void MeshAnimator::calculateBoneTransform( AnimationNode *node, const DirectX::XMFLOAT4X4 &parent_transform )
    {
        const std::string &node_name = node->name;
        DirectX::XMFLOAT4X4 node_transform = node->transform;

        auto it = std::find_if( currentClip.bones.begin(),
            currentClip.bones.end(),
            [node_name]( const Bone &bone ) { return bone.getName() == node_name; } );

        if ( it != currentClip.bones.end() )
        {
            Bone &bone = ( *it );
            bone.onUpdate( tickCount );
            node_transform = bone.getLocalTransform();
        }

        DirectX::XMMATRIX global_transform_mat =
            DirectX::XMLoadFloat4x4( &node_transform ) * DirectX::XMLoadFloat4x4( &parent_transform );
        DirectX::XMFLOAT4X4 global_transform{};
        DirectX::XMStoreFloat4x4( &global_transform, global_transform_mat );

        if ( skinnedMesh->skeletonMap.find( node_name ) != skinnedMesh->skeletonMap.end() )
        {
            Uint32 id = skinnedMesh->skeletonMap[node_name].id;
            SM_ASSERT( id < maxBoneCount, "MeshAnimator::calculateBoneTransform > Max bone count reached" );

            DirectX::XMMATRIX transform_mat =
                DirectX::XMLoadFloat4x4( &skinnedMesh->skeletonMap[node_name].offset ) * global_transform_mat;
            DirectX::XMStoreFloat4x4( &transforms[id], transform_mat );
        }

        for ( Uint32 i{}; i < node->childrenCount; ++i )
            calculateBoneTransform( &node->children[i], global_transform );
    }

    void MeshAnimator::setAnimation( const std::string &clip_name )
    {
        clipSet = false;

        auto it = std::find_if( skinnedMesh->animationClips.begin(),
            skinnedMesh->animationClips.end(),
            [clip_name]( const AnimationClip &clip ) { return clip.name == clip_name; } );

        if ( it != skinnedMesh->animationClips.end() )
            setAnimation( *it );
        else
        {
            reset( false );
            SM_LOG_WARNING( "MeshAnimator::setAnimation > clip with clipName was not found in m_AnimationClips" );
        }
    }

    void MeshAnimator::setAnimation( Uint32 clip_id )
    {
        if ( clip_id < skinnedMesh->animationClips.size() )
        {
            AnimationClip &animation_clip = skinnedMesh->animationClips[clip_id];
            setAnimation( animation_clip );
        }
        else
        {
            reset( false );
            SM_LOG_WARNING( "MeshAnimator::SetAnimation > clipID is bigger than the number of animation clips" );
        }
    }

    void MeshAnimator::setAnimation( const AnimationClip &clip )
    {
        clipSet = true;
        currentClip = clip;
        reset( false );
    }

    void MeshAnimator::reset( bool pause )
    {
        // if (bPause)
        //	Pause();

        ////Set m_TickCount to zero
        // m_TickCount = 0;
        ////Set m_AnimationSpeed to 1.0f
        // m_AnimationSpeed = 1.0f;

        ////If m_ClipSet is true
        ////	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
        ////	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
        ////Else
        ////	Create an IdentityMatrix
        ////	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at
        ///vector::assign)
        // if (m_bClipSet)
        //{
        //	auto boneTransforms = m_CurrentClip.Keys[0].BoneTransforms;
        //	m_Transforms.assign(boneTransforms.begin(), boneTransforms.end());
        //}
        // else
        //{
        //	DirectX::XMFLOAT4X4 identityMatrix{};
        //	DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity{});

        //	m_Transforms.assign(m_pMesh->m_BoneCount, identityMatrix);
        //}
    }
}