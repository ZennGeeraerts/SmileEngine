/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::math
{
    class Color final
    {
      public:
        Color() noexcept = default;

        constexpr Color( const Real red, const Real green, const Real blue, const Real alpha ) noexcept
            : Red{ red }, Green{ green }, Blue{ blue }, Alpha{ alpha }
        {
        }

        constexpr Color &operator=( const Color & ) noexcept = default;

        constexpr bool operator==( const Color &other ) const noexcept
        {
            return Red == other.Red && Green == other.Green && Blue == other.Blue && Alpha == other.Alpha;
        }

        constexpr bool operator!=( const Color &other ) const noexcept
        {
            return !( *this == other );
        }

        constexpr void Set( const Real red, const Real green, const Real blue, const Real alpha ) noexcept
        {
            Red = red;
            Green = green;
            Blue = blue;
            Alpha = alpha;
        }

        constexpr Uint32 GetABGR() const noexcept
        {
            int r = static_cast< int >( Red * 255 );
            int g = static_cast< int >( Green * 255 );
            int b = static_cast< int >( Blue * 255 );
            int a = static_cast< int >( Alpha * 255 );

            return a << 24 | b << 16 | g << 8 | r;
        }

        static constexpr Color GetRed() noexcept
        {
            return { 1.0f, 0.0f, 0.0f, 1.0f };
        }

        static constexpr Color GetGreen() noexcept
        {
            return { 0.0f, 1.0f, 0.0f, 1.0f };
        }

        static constexpr Color GetBlue() noexcept
        {
            return { 0.0f, 0.0f, 1.0f, 1.0f };
        }

        union
        {
            struct
            {
                Real Red, Green, Blue, Alpha;
            };
            struct
            {
                Real r, g, b, a;
            };
        };
    };

    constexpr Color operator+( const Color &first, const Color &second ) noexcept
    {
        return {
            first.Red + second.Red, first.Green + second.Green, first.Blue + second.Blue, first.Alpha + second.Alpha };
    }

    constexpr Color &operator+=( Color &first, const Color &second ) noexcept
    {
        first.Red += second.Red;
        first.Green += second.Green;
        first.Blue += second.Blue;
        first.Alpha += second.Alpha;

        return first;
    }

    constexpr Color operator-( const Color &first, const Color &second ) noexcept
    {
        return {
            first.Red - second.Red, first.Green - second.Green, first.Blue - second.Blue, first.Alpha - second.Alpha };
    }

    constexpr Color &operator-=( Color &first, const Color &second ) noexcept
    {
        first.Red -= second.Red;
        first.Green -= second.Green;
        first.Blue -= second.Blue;
        first.Alpha -= second.Alpha;

        return first;
    }

    constexpr Color operator*( const Color &first, const Color &second ) noexcept
    {
        return {
            first.Red * second.Red, first.Green * second.Green, first.Blue * second.Blue, first.Alpha * second.Alpha };
    }

    constexpr Color &operator*=( Color &first, const Color &second ) noexcept
    {
        first.Red *= second.Red;
        first.Green *= second.Green;
        first.Blue *= second.Blue;
        first.Alpha *= second.Alpha;

        return first;
    }

    constexpr Color operator*( const Color &color, const Real scalar ) noexcept
    {
        return { color.Red * scalar, color.Green * scalar, color.Blue * scalar, color.Alpha * scalar };
    }

    constexpr Color operator*( const Real scalar, const Color &color ) noexcept
    {
        return { color.Red * scalar, color.Green * scalar, color.Blue * scalar, color.Alpha * scalar };
    }

    constexpr Color &operator*=( Color &first, const Real scalar ) noexcept
    {
        first.Red *= scalar;
        first.Green *= scalar;
        first.Blue *= scalar;
        first.Alpha *= scalar;

        return first;
    }

    constexpr Color operator/( const Color &first, const Color &second ) noexcept
    {
        return {
            first.Red / second.Red, first.Green / second.Green, first.Blue / second.Blue, first.Alpha / second.Alpha };
    }

    constexpr Color &operator/=( Color &first, const Color &second ) noexcept
    {
        first.Red /= second.Red;
        first.Green /= second.Green;
        first.Blue /= second.Blue;
        first.Alpha /= second.Alpha;

        return first;
    }

    constexpr Color operator/( const Color &color, const Real scalar ) noexcept
    {
        return { color.Red / scalar, color.Green / scalar, color.Blue / scalar, color.Alpha / scalar };
    }

    constexpr Color operator/( const Real scalar, const Color &color ) noexcept
    {
        return { color.Red / scalar, color.Green / scalar, color.Blue / scalar, color.Alpha / scalar };
    }

    constexpr Color &operator/=( Color &first, const Real scalar ) noexcept
    {
        first.Red /= scalar;
        first.Green /= scalar;
        first.Blue /= scalar;
        first.Alpha /= scalar;

        return first;
    }

    static_assert( sizeof( Color ) == 4 * sizeof( Real ) );
}