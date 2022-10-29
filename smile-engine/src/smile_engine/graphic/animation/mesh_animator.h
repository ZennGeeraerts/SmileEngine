#pragma once
#include "smile_engine/graphic/mesh/skinned_mesh_filter.h"

namespace Smile::Graphic
{
    class MeshAnimator final
    {
      public:
        MeshAnimator( const Ref< SkinnedMeshFilter > &pSkinnedMesh );
        virtual ~MeshAnimator() = default;

        void OnUpdate( Timestep deltaTime );

        void SetAnimation( const std::string &clipName );
        void SetAnimation( Uint32 clipID );
        void SetAnimation( const AnimationClip &clip );

        void Play()
        {
            m_IsPlaying = true;
        }
        void Pause()
        {
            m_IsPlaying = false;
        }
        void Reset( bool pause = true );

        const std::vector< DirectX::XMFLOAT4X4 > &GetBoneTransforms() const
        {
            return m_Transforms;
        }
        bool IsPlaying() const
        {
            return m_IsPlaying;
        }

      private:
        void CalculateBoneTransform( AnimationNode *pNode, const DirectX::XMFLOAT4X4 &parentTransform );

      private:
        AnimationClip m_CurrentClip;
        Ref< SkinnedMeshFilter > m_pSkinnedMesh;
        std::vector< DirectX::XMFLOAT4X4 > m_Transforms;

        float m_TickCount = 0.f;

        bool m_IsClipSet = false;
        bool m_IsPlaying = false;
        bool m_IsReversed = false;

        static const Uint32 s_MaxBoneCount;
    };
}
