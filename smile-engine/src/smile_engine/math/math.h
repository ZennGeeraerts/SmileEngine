#pragma once

#include "smile_engine/core/logger.h"

namespace Smile::Math
{
    constexpr float g_PI = 3.14159265358979323846264338327950288419716939937510f;
    constexpr float g_PI2 = 3.14159265358979323846264338327950288419716939937510f * 2.0f;
    constexpr float g_PIDiv2 = 3.14159265358979323846264338327950288419716939937510f / 2.0f;
    constexpr float g_Epsilon = 0.00001f;

    inline float SquareRoot( float value )
    {
        return sqrt( value );
    }

    inline float Sinus( float value )
    {
        return sin( value );
    }

    inline float Cosinus( float value )
    {
        return cos( value );
    }

    inline float Tangent( float value )
    {
        return tan( value );
    }

    inline float ArcTangent( float y, float x )
    {
        return atan2( y, x );
    }

    template < typename Type >
    inline constexpr Type Sign( Type value )
    {
        return value == 0 ? Type{ 0 } : value > 0 ? Type{ 1 } : Type{ -1 };
    }

    template < typename Type >
    inline constexpr Type Square( Type value )
    {
        return value * value;
    }

    inline float AbsoluteValue( float value )
    {
        return fabs( value );
    }

    inline int AbsoluteValue( int value )
    {
        return ( value >= 0 ) ? value : -value;
    }

    inline int Floor( float value )
    {
        return static_cast< int >( std::floorf( value ) );
    }

    inline int Ceil( float value )
    {
        return static_cast< int >( std::ceilf( value ) );
    }

    inline float Round( float value )
    {
        return std::roundf( value );
    }

    inline constexpr float ToDegrees( float radians )
    {
        return radians * 180.0f / g_PI;
    }

    inline constexpr float ToRadians( float degrees )
    {
        return degrees * g_PI / 180.0f;
    }

    inline bool CompareFloats( float f1, float f2, float precision = g_Epsilon )
    {
        const float deltaF = AbsoluteValue( f1 - f2 );
        return ( deltaF < g_Epsilon );
    }

    inline constexpr bool IsSquareOne( float value, float precision = g_Epsilon )
    {
        return value >= 1.0f - 2.0f * precision + precision * precision &&
               value <= 1.0f + 2.0f * precision + precision * precision;
    }

    inline constexpr bool IsSquareZero( float value, float precision = g_Epsilon )
    {
        return value >= -precision * precision && value <= precision * precision;
    }

    inline constexpr bool IsZero( float value, float precision = g_Epsilon )
    {
        return ( value * value ) < ( precision * precision );
    }

    inline constexpr bool IsOne( float value, float precision = g_Epsilon )
    {
        return IsZero( value - 1.0f, precision );
    }

    float GenerateRandom( float min, float max );
    Int32 GenerateRandomInt( Int32 min, Int32 max );

    inline float Minimum( float f1, float f2 )
    {
        return f1 < f2 ? f1 : f2;
    }

    inline float Maximum( float f1, float f2 )
    {
        return f1 > f2 ? f1 : f2;
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