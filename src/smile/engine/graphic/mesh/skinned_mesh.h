/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/graphic/renderer_api/resource/vertex_buffer.h"
#include "engine/graphic/renderer_api/resource/index_buffer.h"
#include "bone_info.h"

namespace smile::graphic
{
    struct SkinnedMesh final
    {
        Ref< VertexBuffer > pVertexBuffer = nullptr;
        Ref< IndexBuffer > pIndexBuffer = nullptr;
        std::unordered_map< std::string, BoneInfo > SkeletonMap{};
        Uint32 BoneCount = 0;
    };
}