/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::physics::ecs
{
    struct SphereColliderComponent final
    {
        SphereColliderComponent()
        {
            graphic::BufferLayout bufferLayout{ { graphic::ShaderDataType::Float3, "POSITION" } };
            // TODO: Update wireframe mesh when Radius gets updated
            pWireframeMesh = graphic::MeshFactory::CreateSphere( bufferLayout, Radius, 10 );
        }

        SphereColliderComponent( const SphereColliderComponent & ) = default;

        float Radius = 0.5f;
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< physics::PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::Mesh > pWireframeMesh = nullptr;  // TODO: Get rid of mesh, use geometry instead and render geometry
    };
}