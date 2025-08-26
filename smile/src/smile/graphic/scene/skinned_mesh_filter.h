/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "mesh_filter.h"
#include "bone_info.h"

namespace smile::graphic
{
    class SkinnedMeshFilter final : public MeshFilter
    {
      public:
        SkinnedMeshFilter() = default;
        virtual ~SkinnedMeshFilter();

      private:
        std::vector< DirectX::XMFLOAT4 > m_BlendIndices = {};
        std::vector< DirectX::XMFLOAT4 > m_BlendWeights = {};

        std::unordered_map< std::string, BoneInfo > m_SkeletonMap{};
        Uint32 m_BoneCount = 0;

        friend class MeshFactory;
        friend class ModelLoader;
        friend class MeshAnimator;
    };
}
