#pragma once

namespace Smile::Physics
{
    struct PhysicsMaterial final
    {
        float StaticFriction = 0.3f;
        float DynamicFriction = 0.3f;
        float Restitution = 0.3f;
    };
}