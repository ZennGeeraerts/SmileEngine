/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/core/geometric/box.h"
#include "engine/graphic/mesh/mesh_factory.h"

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

        geometric::Box Box{};
        bool IsTrigger = false;
        bool ShowColliderBounds = true;

        Ref< PhysicsMaterial > pPhysicsMaterial = nullptr;
        Ref< graphic::Mesh > pWireframeMesh =
            nullptr; // TODO: Get rid of mesh, use geometry instead and render geometry
    };
}