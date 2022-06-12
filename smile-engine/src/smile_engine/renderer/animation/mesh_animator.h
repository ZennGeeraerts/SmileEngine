#pragma once
#include "smile_engine/renderer/mesh/skinned_mesh_filter.h"

namespace smile::renderer
{
    class MeshAnimator final
    {
      public:
        MeshAnimator( const Ref< SkinnedMeshFilter > &mesh );
        virtual ~MeshAnimator() = default;

        void onUpdate( Timestep delta_time );

        void setAnimation( const std::string &clip_name );
        void setAnimation( Uint32 clip_id );
        void setAnimation( const AnimationClip &clip );

        void play()
        {
            playing = true;
        }
        void pause()
        {
            playing = false;
        }
        void reset( bool pause = true );

        const std::vector< DirectX::XMFLOAT4X4 > &getBoneTransforms() const
        {
            return transforms;
        }
        bool isPlaying() const
        {
            return playing;
        }

      private:
        void calculateBoneTransform( AnimationNode *node, const DirectX::XMFLOAT4X4 &parent_transform );

      private:
        AnimationClip currentClip;
        Ref< SkinnedMeshFilter > skinnedMesh;
        std::vector< DirectX::XMFLOAT4X4 > transforms;

        float tickCount = 0.f;

        bool clipSet = false;
        bool playing = false;
        bool reversed = false;

        static const Uint32 maxBoneCount;
    };
}
