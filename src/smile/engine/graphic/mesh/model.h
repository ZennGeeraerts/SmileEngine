/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh_filter.h"
#include "skinned_mesh_filter.h"
#include "smile/engine/graphic/animation/animation_clip.h"

namespace smile::graphic
{
    class Model final
    {
      public:
        Model() = default;
        ~Model() = default;

        const std::string &GetFilePath() const
        {
            return m_FilePath;
        }

        Ref< MeshFilter > GetMeshFilter( Uint32 index ) const
        {
            SM_ASSERT( index >= 0 && index < m_pMeshes.size(), "Model::GetMeshFilter > Index out of range" );
            return m_pMeshes[index];
        }

        Uint32 GetMeshCount() const
        {
            return m_pMeshes.size();
        }

        Ref< SkinnedMeshFilter > GetSkinnedMeshFilter( Uint32 index ) const
        {
            SM_ASSERT(
                index >= 0 && index < m_pSkinnedMeshes.size(), "Model::GetSkinnedMeshFilter > Index out of range" );
            return m_pSkinnedMeshes[index];
        }

        Uint32 GetSkinnedMeshCount() const
        {
            return m_pSkinnedMeshes.size();
        }

        const std::vector< Ref< AnimationClip > >& GetAnimationClips() const
        {
            return m_pAnimationClips;
        }

      private:
        std::string m_FilePath;
        std::vector< Ref< MeshFilter > > m_pMeshes{};
        std::vector< Ref< SkinnedMeshFilter > > m_pSkinnedMeshes{};
        std::vector< Ref< AnimationClip > > m_pAnimationClips{};

        friend class ModelLoader;
    };
}