/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/mesh/mesh_factory.h"

namespace smile::physics::ecs
{
    struct BoxColliderComponent final
    {
        BoxColliderComponent()
        {
            graphic::BufferLayout bufferLayout{ { graphic::ShaderDataType::Float3, "POSITION" } };
            pWireframeMesh = graphic::MeshFactory::CreateCube( bufferLayout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        DirectX::XMFLOAT3 Size = { 1.0f, 1.0f, 1.0f };
        DirectX::XMFLOAT3 Offset = { 0.0f, 0.0f, 0.0f };
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::Mesh > pWireframeMesh = nullptr; // TODO: Get rid of mesh, use geometry instead and render geometry
    };
}