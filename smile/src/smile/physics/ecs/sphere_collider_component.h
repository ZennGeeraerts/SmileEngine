/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/geometric/sphere.h"

namespace smile::physics::ecs
{
    struct SphereColliderComponent final
    {
        SphereColliderComponent()
        {
            graphic::BufferLayout vertexLayout{ { graphic::Format::RGB32_FLOAT, "POSITION" } };
            // TODO: Update wireframe mesh when Radius gets updated
            pWireframeMesh = graphic::MeshFactory::CreateSphere( vertexLayout, 0.5f, 10 );
        }

        SphereColliderComponent( const SphereColliderComponent & ) = default;

        geometric::Sphere Sphere{ DirectX::XMFLOAT3{ 0.0f, 0.0f, 0.0f }, 0.5f };
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
        // TODO: Get rid of mesh, use geometry instead and render geometry
        Ref< graphic::Mesh > pWireframeMesh = nullptr;
    };
}