/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/logging/logger.h"

#include <DirectXMath.h>

namespace smile::math
{
    constexpr float g_PI = 3.14159265358979323846264338327950288419716939937510f;
    constexpr float g_PI2 = 3.14159265358979323846264338327950288419716939937510f * 2.0f;
    constexpr float g_PIDiv2 = 3.14159265358979323846264338327950288419716939937510f / 2.0f;
    constexpr float g_Epsilon = 0.00001f;

    template < typename Type >
    concept Numeric = std::integral< Type > || std::floating_point< Type >;

    template < Numeric Type >
    inline Type SquareRoot( Type value ) noexcept
    {
        return static_cast< Type >( sqrt( value ) );
    }

    template < Numeric Type >
    inline Type Sinus( Type value ) noexcept
    {
        return static_cast< Type >( sin( value ) );
    }

    template < Numeric Type >
    inline Type Cosinus( Type value ) noexcept
    {
        return static_cast< Type >( cos( value ) );
    }

    template < Numeric Type >
    inline Type ArcCosinus( Type value ) noexcept
    {
        return static_cast< Type >( acos( value ) );
    }

    template < Numeric Type >
    inline Type Tangent( Type value ) noexcept
    {
        return static_cast< Type >( tan( value ) );
    }

    template < Numeric Type >
    inline Type ArcTangent( Type y, Type x ) noexcept
    {
        return static_cast< Type >( atan2( y, x ) );
    }

    template < Numeric Type >
    constexpr Type Sign( Type value ) noexcept
    {
        return value == 0 ? Type{ 0 } : value > 0 ? Type{ 1 } : Type{ -1 };
    }

    template < Numeric Type >
    constexpr Type Square( Type value ) noexcept
    {
        return value * value;
    }

    template < Numeric Type >
    inline Type AbsoluteValue( Type value ) noexcept
    {
        return static_cast< Type >( std::abs( value ) );
    }

    template < Numeric Type >
    inline int Floor( Type value ) noexcept
    {
        return static_cast< int >( std::floor( value ) );
    }

    template < Numeric Type >
    inline int Ceil( Type value ) noexcept
    {
        return static_cast< int >( std::ceil( value ) );
    }

    template < Numeric Type >
    inline Type Round( Type value ) noexcept
    {
        return static_cast< Type >( std::round( value ) );
    }

    template < Numeric Type >
    constexpr Type ToDegrees( Type radians ) noexcept
    {
        return radians * static_cast< Type >( 180.0f ) / static_cast< Type >( g_PI );
    }

    template < Numeric Type >
    constexpr Type ToRadians( Type degrees ) noexcept
    {
        return degrees * static_cast< Type >( g_PI ) / static_cast< Type >( 180.0f );
    }

    template < Numeric Type >
    inline bool AreEqual( Type a, Type b ) noexcept
    {
        const Type delta = AbsoluteValue( a - b );
        return delta <= std::numeric_limits< Type >::epsilon();
    }

    constexpr bool IsSquareOne( float value, float precision = g_Epsilon ) noexcept
    {
        return value >= 1.0f - 2.0f * precision + precision * precision &&
               value <= 1.0f + 2.0f * precision + precision * precision;
    }

    constexpr bool IsSquareZero( float value, float precision = g_Epsilon ) noexcept
    {
        return value >= -precision * precision && value <= precision * precision;
    }

    constexpr bool IsZero( float value, float precision = g_Epsilon ) noexcept
    {
        return ( value * value ) < ( precision * precision );
    }

    constexpr bool IsOne( float value, float precision = g_Epsilon ) noexcept
    {
        return IsZero( value - 1.0f, precision );
    }

    float GenerateRandom( float min, float max ) noexcept;
    Int32 GenerateRandomInt( Int32 min, Int32 max ) noexcept;

    template < Numeric Type >
    constexpr Type Minimum( Type a, Type b ) noexcept
    {
        return a < b ? a : b;
    }

    template < Numeric Type >
    constexpr Type Maximum( Type a, Type b ) noexcept
    {
        return a > b ? a : b;
    }

    // Matrix
    struct Matrix4x4 final
    {
        union
        {
            float m[4][4];
            struct
            {
                float m00, m01, m02, m03;
                float m10, m11, m12, m13;
                float m20, m21, m22, m23;
                float m30, m31, m32, m33;
            };
        };
    };

    struct Matrix4x3 final
    {
        union
        {
            float m[4][3];
            struct
            {
                float m00, m01, m02;
                float m10, m11, m12;
                float m20, m21, m22;
                float m30, m31, m32;
            };
        };
    };

    struct Matrix4x1 final
    {
        union
        {
            float m[4];
            struct
            {
                float m00, m01, m02, m03;
            };
        };
    };

    DirectX::XMFLOAT3 QuaternionToEuler( const DirectX::XMFLOAT4 &quaternion );
    bool DecomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale );
}