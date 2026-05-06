/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "mesh_source.h"
#include "bone_info.h"
#include "smile/common/primitive/collection/hash_map.h"

namespace smile::graphic
{
    class SkinnedMeshSource final : public MeshSource
    {
      public:
        SkinnedMeshSource() = default;
        SkinnedMeshSource( const primitive::String &name ) noexcept : MeshSource{ name }
        {
        }

        virtual ~SkinnedMeshSource() = default;

      private:
        primitive::Vector< DirectX::XMFLOAT4 > m_BlendIndices = {};
        primitive::Vector< DirectX::XMFLOAT4 > m_BlendWeights = {};

        primitive::HashMap< primitive::String, BoneInfo > m_SkeletonMap{};
        Count m_BoneCount = 0;

        friend class MeshFactory;
        friend class ModelLoader;
        friend class MeshAnimator;
    };
}
