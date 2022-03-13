#pragma once

namespace smile
{
    struct PhysicsMaterial final
    {
        float m_StaticFriction = 0.3f;
        float m_DynamicFriction = 0.3f;
        float m_Restitution = 0.3f;
    };
}