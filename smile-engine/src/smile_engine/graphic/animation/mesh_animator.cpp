#include "smpch.h"
#include "mesh_animator.h"

namespace Smile::Graphic
{
    const Uint32 MeshAnimator::s_MaxBoneCount{ 70 };

    MeshAnimator::MeshAnimator( const Ref< SkinnedMeshFilter > &pSkinnedMesh ) : m_pSkinnedMesh{ pSkinnedMesh }
    {
        m_Transforms.resize( s_MaxBoneCount );
    }

    void MeshAnimator::OnUpdate( Timestep deltaTime )
    {
        if ( m_IsClipSet && m_IsPlaying )
        {
            float passedTicks = deltaTime * m_CurrentClip.ticksPerSecond;
            passedTicks = fmod( passedTicks, m_CurrentClip.duration );

            if ( !m_IsReversed )
            {
                m_TickCount += passedTicks;
                if ( m_TickCount > m_CurrentClip.duration )
                    m_TickCount = 0;
            }
            else
            {
                m_TickCount -= passedTicks;
                if ( m_TickCount < 0 )
                    m_TickCount = m_CurrentClip.duration;
            }

            DirectX::XMFLOAT4X4 identityMatrix{};
            DirectX::XMStoreFloat4x4( &identityMatrix, DirectX::XMMatrixIdentity() );
            CalculateBoneTransform( &m_CurrentClip.rootNode, identityMatrix );
        }
    }

    void MeshAnimator::CalculateBoneTransform( AnimationNode *pNode, const DirectX::XMFLOAT4X4 &parentTransform )
    {
        const std::string &nodeName = pNode->name;
        DirectX::XMFLOAT4X4 nodeTransform = pNode->transform;

        auto it = std::find_if( m_CurrentClip.bones.begin(),
            m_CurrentClip.bones.end(),
            [nodeName]( const Bone &bone ) { return bone.GetName() == nodeName; } );

        if ( it != m_CurrentClip.bones.end() )
        {
            Bone &bone = ( *it );
            bone.OnUpdate( m_TickCount );
            nodeTransform = bone.GetLocalTransform();
        }

        DirectX::XMMATRIX globalTransformMat =
            DirectX::XMLoadFloat4x4( &nodeTransform ) * DirectX::XMLoadFloat4x4( &parentTransform );
        DirectX::XMFLOAT4X4 globalTransform{};
        DirectX::XMStoreFloat4x4( &globalTransform, globalTransformMat );

        if ( m_pSkinnedMesh->m_SkeletonMap.find( nodeName ) != m_pSkinnedMesh->m_SkeletonMap.end() )
        {
            Uint32 id = m_pSkinnedMesh->m_SkeletonMap[nodeName].id;
            SM_ASSERT( id < s_MaxBoneCount, "MeshAnimator::calculateBoneTransform > Max bone count reached" );

            DirectX::XMMATRIX transformMat =
                DirectX::XMLoadFloat4x4( &m_pSkinnedMesh->m_SkeletonMap[nodeName].offset ) * globalTransformMat;
            DirectX::XMStoreFloat4x4( &m_Transforms[id], transformMat );
        }

        for ( Uint32 i{}; i < pNode->childrenCount; ++i )
            CalculateBoneTransform( &pNode->children[i], globalTransform );
    }

    void MeshAnimator::SetAnimation( const std::string &clipName )
    {
        m_IsClipSet = false;

        auto it = std::find_if( m_pSkinnedMesh->m_AnimationClips.begin(),
            m_pSkinnedMesh->m_AnimationClips.end(),
            [clipName]( const AnimationClip &clip ) { return clip.name == clipName; } );

        if ( it != m_pSkinnedMesh->m_AnimationClips.end() )
            SetAnimation( *it );
        else
        {
            Reset( false );
            SM_LOG_WARNING( "MeshAnimator::setAnimation > clip with clipName was not found in m_AnimationClips" );
        }
    }

    void MeshAnimator::SetAnimation( Uint32 clipID )
    {
        if ( clipID < m_pSkinnedMesh->m_AnimationClips.size() )
        {
            AnimationClip &animation_clip = m_pSkinnedMesh->m_AnimationClips[clipID];
            SetAnimation( animation_clip );
        }
        else
        {
            Reset( false );
            SM_LOG_WARNING( "MeshAnimator::SetAnimation > clipID is bigger than the number of animation clips" );
        }
    }

    void MeshAnimator::SetAnimation( const AnimationClip &clip )
    {
        m_IsClipSet = true;
        m_CurrentClip = clip;
        Reset( false );
    }

    void MeshAnimator::Reset( bool pause )
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