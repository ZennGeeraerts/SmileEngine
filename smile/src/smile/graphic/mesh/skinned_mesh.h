/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_api/resource/vertex_buffer.h"
#include "smile/graphic/renderer_api/resource/index_buffer.h"
#include "bone_info.h"
#include "memory/ref.h"

namespace smile::graphic
{
    struct SkinnedMesh final
    {
        memory::Ref< VertexBuffer > pVertexBuffer = nullptr;
        memory::Ref< IndexBuffer > pIndexBuffer = nullptr;
        std::unordered_map< std::string, BoneInfo > SkeletonMap{};
        Uint32 BoneCount = 0;
    };
}