#pragma once

namespace smile::physics
{
    struct PhysicsMaterial final
    {
        float staticFriction = 0.3f;
        float dynamicFriction = 0.3f;
        float restitution = 0.3f;
    };
}