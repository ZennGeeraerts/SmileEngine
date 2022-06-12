#pragma once

#include "smile_engine/core/logger.h"

namespace smile::math
{
    constexpr float pi = 3.14159265358979323846264338327950288419716939937510f;
    constexpr float pi2 = 3.14159265358979323846264338327950288419716939937510f * 2.0f;
    constexpr float piDiv2 = 3.14159265358979323846264338327950288419716939937510f / 2.0f;
    constexpr float epsilon = 0.00001f;

    inline float squareRoot( float value )
    {
        return sqrt( value );
    }

    inline float sinus( float value )
    {
        return sin( value );
    }

    inline float cosinus( float value )
    {
        return cos( value );
    }

    inline float tangent( float value )
    {
        return tan( value );
    }

    inline float arcTangent( float y, float x )
    {
        return atan2( y, x );
    }

    template < typename Type >
    inline constexpr Type sign( Type value )
    {
        return value == 0 ? Type{ 0 } : value > 0 ? Type{ 1 } : Type{ -1 };
    }

    template < typename Type >
    inline constexpr Type square( Type value )
    {
        return value * value;
    }

    inline float absoluteValue( float value )
    {
        return fabs( value );
    }

    inline int absoluteValue( int value )
    {
        return ( value >= 0 ) ? value : -value;
    }

    inline int floor( float value )
    {
        return static_cast< int >( std::floorf( value ) );
    }

    inline int ceil( float value )
    {
        return static_cast< int >( std::ceilf( value ) );
    }

    inline float round( float value )
    {
        return std::roundf( value );
    }

    inline constexpr float toDegrees( float radians )
    {
        return radians * 180.0f / pi;
    }

    inline constexpr float toRadians( float degrees )
    {
        return degrees * pi / 180.0f;
    }

    inline bool compareFloats( float f1, float f2, float precision = epsilon )
    {
        const float delta_f = absoluteValue( f1 - f2 );
        return ( delta_f < epsilon );
    }

    inline constexpr bool isSquareOne( float value, float precision = epsilon )
    {
        return value >= 1.0f - 2.0f * precision + precision * precision &&
               value <= 1.0f + 2.0f * precision + precision * precision;
    }

    inline constexpr bool isSquareZero( float value, float precision = epsilon )
    {
        return value >= -precision * precision && value <= precision * precision;
    }

    inline constexpr bool isZero( float value, float precision = epsilon )
    {
        return ( value * value ) < ( precision * precision );
    }

    inline constexpr bool isOne( float value, float precision = epsilon )
    {
        return isZero( value - 1.0f, precision );
    }

    float generateRandom( float min, float max );
    Int32 generateRandomInt( Int32 min, Int32 max );

    inline float minimum( float f1, float f2 )
    {
        return f1 < f2 ? f1 : f2;
    }

    inline float maximum( float f1, float f2 )
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

    DirectX::XMFLOAT3 quaternionToEuler( const DirectX::XMFLOAT4 &quaternion );
    bool decomposeMatrix( const DirectX::XMFLOAT4X4 &transform,
        DirectX::XMFLOAT3 &translation,
        DirectX::XMFLOAT3 &rotation,
        DirectX::XMFLOAT3 &scale );
}