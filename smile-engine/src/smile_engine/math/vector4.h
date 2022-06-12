#pragma once

#include "math.h"

namespace smile::math
{
    struct Vector4 final
    {
        union
        {
            float m[4];
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };

        static const Vector4 zero, one;
    };

    inline float dotProduct( const Vector4 &v1, const Vector4 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    inline float distanceSqr( const Vector4 &v1, const Vector4 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w );
    }

    inline float distance( const Vector4 &v1, const Vector4 &v2 )
    {
        return squareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
                           ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w ) );
    }

    inline float lengthSqr( const Vector4 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
    }

    inline float length( const Vector4 &vector )
    {
        return squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    inline bool isUnitVector( const Vector4 &vector )
    {
        return isOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    inline bool hasZeroLength( const Vector4 &vector, float precision = epsilon )
    {
        return isSquareZero(
            vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w, precision );
    }

    inline bool isZeroVector( const Vector4 &vector, float precision = epsilon )
    {
        return isZero( vector.x, precision ) && isZero( vector.y, precision ) && isZero( vector.z, precision ) &&
               isZero( vector.w, precision );
    }

    inline bool compareVector4( const Vector4 &v1, const Vector4 &v2, float precision = epsilon )
    {
        return compareFloats( v1.x, v2.x, precision ) && compareFloats( v1.y, v2.y, precision ) &&
               compareFloats( v1.z, v2.z, precision ) && compareFloats( v1.w, v2.w, precision );
    }

    inline Vector4 getNormalized( const Vector4 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "GetNormalized > Vector4 is zero vector" );

        const float length =
            squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );

        return Vector4{ vector.x / length, vector.y / length, vector.z / length, vector.w / length };
    }

    inline void normalize( Vector4 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "Normalize > Vector4 is zero vector" );

        const float length =
            squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
        vector.w /= length;
    }

    inline Vector4 lerp( const Vector4 &v1, const Vector4 &v2, float percentage )
    {
        const float one_minus_percentage = 1.0f - percentage;

        return Vector4{ v1.x * one_minus_percentage + v2.x * percentage,
            v1.y * one_minus_percentage + v2.y * percentage,
            v1.z * one_minus_percentage + v2.z * percentage,
            v1.w * one_minus_percentage + v2.w * percentage };
    }

    // Operators

    inline bool operator==( const Vector4 &lhs, const Vector4 &rhs )
    {
        return ( lhs.x == rhs.x ) && ( lhs.y == rhs.y ) && ( lhs.z == rhs.z ) && ( lhs.w == rhs.w );
    }

    inline bool operator!=( const Vector4 &lhs, const Vector4 &rhs )
    {
        return ( lhs.x != rhs.x ) || ( lhs.y != rhs.y ) || ( lhs.z != rhs.z ) || ( lhs.w != rhs.w );
    }

    inline Vector4 operator+( const Vector4 &lhs, const Vector4 &rhs )
    {
        return Vector4{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
    }

    inline Vector4 &operator+=( Vector4 &lhs, const Vector4 &rhs )
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        lhs.w += rhs.w;
        return lhs;
    }

    inline Vector4 operator-( const Vector4 &lhs, const Vector4 &rhs )
    {
        return Vector4{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
    }

    inline Vector4 &operator-=( Vector4 &lhs, const Vector4 &rhs )
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        lhs.w -= rhs.w;
        return lhs;
    }

    inline Vector4 operator*( const Vector4 &lhs, const Vector4 &rhs )
    {
        return Vector4{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
    }

    inline Vector4 operator*( const Vector4 &lhs, float rhs )
    {
        return Vector4{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
    }

    inline Vector4 &operator*=( Vector4 &lhs, const Vector4 &rhs )
    {
        lhs.x *= rhs.x;
        lhs.y *= rhs.y;
        lhs.z *= rhs.z;
        lhs.w *= rhs.w;
        return lhs;
    }

    inline Vector4 &operator*=( Vector4 &lhs, float rhs )
    {
        lhs.x *= rhs;
        lhs.y *= rhs;
        lhs.z *= rhs;
        lhs.w *= rhs;
        return lhs;
    }

    inline Vector4 operator/( const Vector4 &lhs, const Vector4 &rhs )
    {
        return Vector4{ lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w };
    }

    inline Vector4 operator/( const Vector4 &lhs, float rhs )
    {
        return Vector4{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
    }

    inline Vector4 &operator/=( Vector4 &lhs, const Vector4 &rhs )
    {
        lhs.x /= rhs.x;
        lhs.y /= rhs.y;
        lhs.z /= rhs.z;
        lhs.w /= rhs.w;
        return lhs;
    }

    inline Vector4 &operator/=( Vector4 &lhs, float rhs )
    {
        lhs.x /= rhs;
        lhs.y /= rhs;
        lhs.z /= rhs;
        lhs.w /= rhs;
        return lhs;
    }

    inline Vector4 operator-( const Vector4 &vector )
    {
        return Vector4{ -vector.x, -vector.y, -vector.z, -vector.w };
    }
}