/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "skinned_mesh_filter.h"

namespace smile::graphic
{
    SkinnedMeshFilter::~SkinnedMeshFilter()
    {
        m_BlendIndices.clear();
        m_BlendWeights.clear();
        m_SkeletonMap.clear();
    }
}
