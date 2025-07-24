/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::math
{
    struct Color final
    {
        Color() = default;

        constexpr Color( const float red, const float green, const float blue, const float alpha )
            : Red{ red }, Green{ green }, Blue{ blue }, Alpha{ alpha }
        {
        }

        constexpr Color &operator=( const Color & ) = default;

        constexpr bool operator==( const Color &other ) const
        {
            return Red == other.Red && Green == other.Green && Blue == other.Blue && Alpha == other.Alpha;
        }

        constexpr bool operator!=( const Color &other ) const
        {
            return !( *this == other );
        }

        constexpr inline void Set( const float red, const float green, const float blue, const float alpha )
        {
            Red = red;
            Green = green;
            Blue = blue;
            Alpha = alpha;
        }

        constexpr Uint32 GetABGR() const
        {
            int r = static_cast< int >( Red * 255 );
            int g = static_cast< int >( Green * 255 );
            int b = static_cast< int >( Blue * 255 );
            int a = static_cast< int >( Alpha * 255 );

            return a << 24 | b << 16 | g << 8 | r;
        }

        static constexpr Color Red()
        {
            return { 1.0f, 0.0f, 0.0f, 1.0f };
        }

        static constexpr Color Green()
        {
            return { 0.0f, 1.0f, 0.0f, 1.0f };
        }

        static constexpr Color Blue()
        {
            return { 0.0f, 0.0f, 1.0f, 1.0f };
        }

        union
        {
            struct
            {
                float Red, Green, Blue, Alpha;
            };
            struct
            {
                float r, g, b, a;
            };
        };
    };

    inline Color operator+( const Color &first, const Color &second )
    {
        return {
            first.Red + second.Red, first.Green + second.Green, first.Blue + second.Blue, first.Alpha + second.Alpha };
    }

    inline Color &operator+=( Color &first, const Color &second )
    {
        first.Red += second.Red;
        first.Green += second.Green;
        first.Blue += second.Blue;
        first.Alpha += second.Alpha;

        return first;
    }

    inline Color operator-( const Color &first, const Color &second )
    {
        return {
            first.Red - second.Red, first.Green - second.Green, first.Blue - second.Blue, first.Alpha - second.Alpha };
    }

    inline Color operator-=( Color &first, const Color &second )
    {
        first.Red -= second.Red;
        first.Green -= second.Green;
        first.Blue -= second.Blue;
        first.Alpha -= second.Alpha;

        return first;
    }

    inline Color operator*( const Color &first, const Color &second )
    {
        return {
            first.Red * second.Red, first.Green * second.Green, first.Blue * second.Blue, first.Alpha * second.Alpha };
    }

    inline Color &operator*=( Color &first, const Color &second )
    {
        first.Red *= second.Red;
        first.Green *= second.Green;
        first.Blue *= second.Blue;
        first.Alpha *= second.Alpha;

        return first;
    }

    inline Color operator*( const Color &color, const float scalar )
    {
        return { color.Red * scalar, color.Green * scalar, color.Blue * scalar, color.Alpha * scalar };
    }

    inline Color operator*( const float scalar, const Color &color )
    {
        return { color.Red * scalar, color.Green * scalar, color.Blue * scalar, color.Alpha * scalar };
    }

    inline Color &operator*=( Color &first, const float scalar )
    {
        first.Red *= scalar;
        first.Green *= scalar;
        first.Blue *= scalar;
        first.Alpha *= scalar;

        return first;
    }

    inline Color operator/( const Color &first, const Color &second )
    {
        return {
            first.Red / second.Red, first.Green / second.Green, first.Blue / second.Blue, first.Alpha / second.Alpha };
    }

    inline Color &operator/=( Color &first, const Color &second )
    {
        first.Red /= second.Red;
        first.Green /= second.Green;
        first.Blue /= second.Blue;
        first.Alpha /= second.Alpha;

        return first;
    }

    inline Color operator/( const Color &color, const float scalar )
    {
        return { color.Red / scalar, color.Green / scalar, color.Blue / scalar, color.Alpha / scalar };
    }

    inline Color operator/( const float scalar, const Color &color )
    {
        return { color.Red / scalar, color.Green / scalar, color.Blue / scalar, color.Alpha / scalar };
    }

    inline Color &operator/=( Color &first, const float scalar )
    {
        first.Red /= scalar;
        first.Green /= scalar;
        first.Blue /= scalar;
        first.Alpha /= scalar;

        return first;
    }

    static_assert( sizeof( Color ) == 4 * sizeof( float ) );
}