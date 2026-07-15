/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh_source.h"
#include "skinned_mesh_source.h"
#include "smile/graphic/animation/animation_clip.h"

namespace smile::graphic
{
    class Model final : public asset::Asset
    {
      public:
        using Ref = memory::Ref< Model >;
        using ConstRef = memory::Ref< const Model >;

        Model() = default;
        ~Model() = default;

        const MeshSource &GetMeshSource( const Index index ) const
        {
            SM_ASSERT_MSG( m_SkinnedMeshes.IsValidIndex( index ), "Model::GetMeshSource > Index out of range" );
            return m_Meshes[index];
        }

        Count GetMeshCount() const noexcept
        {
            return m_Meshes.GetItemCount();
        }

        const SkinnedMeshSource &GetSkinnedMeshSource( const Index index ) const
        {
            SM_ASSERT_MSG( m_SkinnedMeshes.IsValidIndex( index ), "Model::GetSkinnedMeshSource > Index out of range" );
            return m_SkinnedMeshes[index];
        }

        Count GetSkinnedMeshCount() const noexcept
        {
            return m_SkinnedMeshes.GetItemCount();
        }

        const primitive::Vector< AnimationClip > &GetAnimationClips() const noexcept
        {
            return m_AnimationClips;
        }

      private:
        primitive::Vector< MeshSource > m_Meshes;
        primitive::Vector< SkinnedMeshSource > m_SkinnedMeshes;
        primitive::Vector< AnimationClip > m_AnimationClips;

        friend class ModelLoader;
    };
}