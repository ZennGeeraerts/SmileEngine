/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/geometric/box.h"
#include "smile/physics/physics_material.h"
#include "smile/graphic/scene/mesh_factory.h"

namespace smile::physics::ecs
{
    struct BoxColliderComponent final
    {
        BoxColliderComponent()
        {
            graphic::rhi::BufferLayout vertexLayout{ { graphic::rhi::Format::RGB32_FLOAT, "POSITION" } };
            pWireframeMesh = graphic::MeshFactory::CreateCube( vertexLayout );
        }

        BoxColliderComponent( const BoxColliderComponent & ) = default;

        geometric::Box Box{};
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::Mesh > pWireframeMesh =
            nullptr; // TODO: Get rid of mesh, use geometry instead and render geometry
    };
}