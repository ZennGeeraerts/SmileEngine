#pragma once
#include "SmileEngine/Renderer/Mesh/SkinnedMeshFilter.h"

namespace smile
{
    class MeshAnimator final
    {
      public:
        MeshAnimator( const Ref< SkinnedMeshFilter > &pMesh );
        virtual ~MeshAnimator() = default;

        void OnUpdate( Timestep deltaTime );

        void SetAnimation( const std::string &clipName );
        void SetAnimation( uint32_t clipID );
        void SetAnimation( const AnimationClip &clip );

        void Play() { m_bPlaying = true; }
        void Pause() { m_bPlaying = false; }
        void Reset( bool bPause = true );

        const std::vector< DirectX::XMFLOAT4X4 > &GetBoneTransforms() const { return m_Transforms; }
        bool IsPlaying() const { return m_bPlaying; }

      private:
        void CalculateBoneTransform( AnimationNode *pNode, const DirectX::XMFLOAT4X4 &parentTransform );

      private:
        AnimationClip m_CurrentClip;
        Ref< SkinnedMeshFilter > m_pSkinnedMesh;
        std::vector< DirectX::XMFLOAT4X4 > m_Transforms;

        float m_TickCount = 0.f;

        bool m_bClipSet = false;
        bool m_bPlaying = false;
        bool m_bReversed = false;

        static const Uint32 m_MaxBoneCount;
    };
}
