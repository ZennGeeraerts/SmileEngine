#pragma once

#include "smile_engine/core/core.h"

#include <vector>

namespace smile::ecs
{
    struct ComponentBuffer final
    {
        size_t m_TypeID{};
        Uint32 m_TypeSize{};
        std::vector< Byte > m_Data;
    };
}