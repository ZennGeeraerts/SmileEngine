/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/core/geometric/rectangle.h"

namespace smile::graphic::rhi
{
    struct Viewport final
    {
        Viewport() : MinX{ 0.0f }, MaxX{ 0.0f }, MinY{ 0.0f }, MaxY{ 0.0f }, MinZ{ 0.0f }, MaxZ{ 1.0f }
        {
        }

        Viewport( const float width, const float height )
            : MinX{ 0.0f }, MaxX{ width }, MinY{ 0.0f }, MaxY{ height }, MinZ{ 0.0f }, MaxZ{ 1.0f }
        {
        }

        Viewport( const float minX,
            const float maxX,
            const float minY,
            const float maxY,
            const float minZ,
            const float maxZ )
            : MinX{ minX }, MaxX{ maxX }, MinY{ minY }, MaxY{ maxY }, MinZ{ minZ }, MaxZ{ maxZ }
        {
        }

        Viewport( const geometric::Rectangle &rect )
            : MinX{ rect.x },
              MaxX{ rect.x + rect.Width },
              MinY{ rect.y },
              MaxY{ rect.y + rect.Height },
              MinZ{ 0.0f },
              MaxZ{ 0.0f }
        {
        }

        inline bool operator==( const Viewport &other ) const
        {
            return MinX == other.MinX && MinY == other.MinY && MinZ == other.MinZ && MaxX == other.MaxX &&
                   MaxY == other.MaxY && MaxZ == other.MaxZ;
        }

        inline bool operator!=( const Viewport &other )
        {
            return !( *this == other );
        }

        [[nodiscard]] float GetWidth() const
        {
            return MaxX - MinX;
        }

        [[nodiscard]] float GetHeight() const
        {
            return MaxY - MinY;
        }

        float MinX, MaxX;
        float MinY, MaxY;
        float MinZ, MaxZ;
    };
}