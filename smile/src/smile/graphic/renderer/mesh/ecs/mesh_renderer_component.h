/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer/mesh/mesh_source.h"
#include "smile/graphic/renderer/material/asset/material_instance_asset.h"

namespace smile::graphic::ecs
{
    struct MeshRendererComponent final
    {
        MeshRendererComponent() = default;
        MeshRendererComponent( const MeshRendererComponent & ) = default;

        MeshSource::Ref Mesh = nullptr;
        MaterialInstanceAsset::Ref Material = nullptr;
    };
}