#pragma once

#include "SmileEngine/Core/Logger.h"

namespace smile::Math
{
    constexpr float g_PI = 3.14159265358979323846264338327950288419716939937510f;
    constexpr float g_PI2 = 3.14159265358979323846264338327950288419716939937510f * 2.0f;
    constexpr float g_PI_DIV_2 = 3.14159265358979323846264338327950288419716939937510f / 2.0f;
    constexpr float g_EPSILON = 0.00001f;

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

    inline bool CompareFloats( float f1, float f2 )
    {
        float deltaF = AbsoluteValue( f1 - f2 );
        return ( deltaF < g_EPSILON );
    }

    struct Vector3 final
    {
        union
        {
            float M[3];
            struct
            {
                float x;
                float y;
                float z;
            };
        };
    };

    struct Vector4 final
    {
        union
        {
            float M[4];
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };
    };

    // Matrix
    struct Matrix4x4 final
    {
        union
        {
            float M[4][4];
            struct
            {
                float M00, M01, M02, M03;
                float M10, M11, M12, M13;
                float M20, M21, M22, M23;
                float M30, M31, M32, M33;
            };
        };
    };

    struct Matrix4x3 final
    {
        union
        {
            float M[4][3];
            struct
            {
                float M00, M01, M02;
                float M10, M11, M12;
                float M20, M21, M22;
                float M30, M31, M32;
            };
        };
    };

    struct Matrix4x1 final
    {
        union
        {
            float M[4];
            struct
            {
                float M00, M01, M02, M03;
            };
        };
    };

    DirectX::XMFLOAT3 QuaternionToEuler( const DirectX::XMFLOAT4 &quaternion );
    bool DecomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale );
}