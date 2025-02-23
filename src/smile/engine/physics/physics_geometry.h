/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "geometric/box.h"
#include "geometric/sphere.h"
#include "geometric/capsule.h"

namespace smile::physics
{
    enum class PhysicsGeometryType
    {
        Undefined = 0,
        Box = 1,
        Sphere = 2,
        Capsule = 3
    };

    struct PhysicsGeometry
    {
        PhysicsGeometry( PhysicsGeometryType type ) : Type{ type }
        {
        }

        PhysicsGeometryType Type = PhysicsGeometryType::Undefined;
    };

    struct PhysicsBoxGeometry final : public PhysicsGeometry
    {
        PhysicsBoxGeometry( const geometric::Box &box ) : Box{ box }, PhysicsGeometry{ PhysicsGeometryType::Box }
        {
        }

        geometric::Box Box;
    };

    struct PhysicsSphereGeometry final : public PhysicsGeometry
    {
        PhysicsSphereGeometry( const geometric::Sphere &sphere )
            : Sphere{ sphere }, PhysicsGeometry{ PhysicsGeometryType::Sphere }
        {
        }

        geometric::Sphere Sphere;
    };

    struct PhysicsCapsuleGeometry final : public PhysicsGeometry
    {
        PhysicsCapsuleGeometry( const geometric::Capsule &capsule )
            : Capsule{ capsule }, PhysicsGeometry{ PhysicsGeometryType::Capsule }
        {
        }

        geometric::Capsule Capsule;
    };
}