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
        constexpr Viewport() noexcept
            : MinX{ 0.0f }, MaxX{ 0.0f }, MinY{ 0.0f }, MaxY{ 0.0f }, MinZ{ 0.0f }, MaxZ{ 1.0f }
        {
        }

        constexpr Viewport( const float width, const float height ) noexcept
            : MinX{ 0.0f }, MaxX{ width }, MinY{ 0.0f }, MaxY{ height }, MinZ{ 0.0f }, MaxZ{ 1.0f }
        {
        }

        constexpr Viewport( const float minX,
            const float maxX,
            const float minY,
            const float maxY,
            const float minZ,
            const float maxZ ) noexcept
            : MinX{ minX }, MaxX{ maxX }, MinY{ minY }, MaxY{ maxY }, MinZ{ minZ }, MaxZ{ maxZ }
        {
        }

        Viewport( const geometric::Rectangle &rect ) noexcept
            : MinX{ rect.x },
              MaxX{ rect.x + rect.Width },
              MinY{ rect.y },
              MaxY{ rect.y + rect.Height },
              MinZ{ 0.0f },
              MaxZ{ 0.0f }
        {
        }

        bool operator==( const Viewport &other ) const noexcept
        {
            return MinX == other.MinX && MinY == other.MinY && MinZ == other.MinZ && MaxX == other.MaxX &&
                   MaxY == other.MaxY && MaxZ == other.MaxZ;
        }

        bool operator!=( const Viewport &other ) const noexcept
        {
            return !( *this == other );
        }

        float GetWidth() const noexcept
        {
            return MaxX - MinX;
        }

        float GetHeight() const noexcept
        {
            return MaxY - MinY;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< float >{}( MinX );
            hash = foundation::HashCombine( hash, std::hash< float >{}( MaxX ) );
            hash = foundation::HashCombine( hash, std::hash< float >{}( MinY ) );
            hash = foundation::HashCombine( hash, std::hash< float >{}( MaxY ) );
            hash = foundation::HashCombine( hash, std::hash< float >{}( MinZ ) );
            hash = foundation::HashCombine( hash, std::hash< float >{}( MaxZ ) );
            return hash;
        }

        float MinX, MaxX;
        float MinY, MaxY;
        float MinZ, MaxZ;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::Viewport >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::Viewport &viewport ) const
        {
            return viewport.GetHashCode();
        }
    };
}