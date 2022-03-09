#pragma once

#include "Math.h"

namespace smile::math
{
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

        static const Vector4 Zero, One;
    };

    inline float DotProduct( const Vector4 &v1, const Vector4 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    inline float DistanceSqr( const Vector4 &v1, const Vector4 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w );
    }

    inline float Distance( const Vector4 &v1, const Vector4 &v2 )
    {
        return SquareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
                           ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w ) );
    }

    inline float LengthSqr( const Vector4 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
    }

    inline float Length( const Vector4 &vector )
    {
        return SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    inline bool IsUnitVector( const Vector4 &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    inline bool HasZeroLength( const Vector4 &vector, float precision = g_EPSILON )
    {
        return IsSquareZero(
            vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w, precision );
    }

    inline bool IsZeroVector( const Vector4 &vector, float precision = g_EPSILON )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision ) &&
               IsZero( vector.w, precision );
    }

    inline bool CompareVector4( const Vector4 &v1, const Vector4 &v2, float precision = g_EPSILON )
    {
        return CompareFloats( v1.x, v2.x, precision ) && CompareFloats( v1.y, v2.y, precision ) &&
               CompareFloats( v1.z, v2.z, precision ) && CompareFloats( v1.w, v2.w, precision );
    }

    inline Vector4 GetNormalized( const Vector4 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "GetNormalized > Vector4 is zero vector" );

        float length =
            SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );

        return Vector4{ vector.x / length, vector.y / length, vector.z / length, vector.w / length };
    }

    inline void Normalize( Vector4 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "Normalize > Vector4 is zero vector" );

        float length =
            SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
        vector.w /= length;
    }

    inline Vector4 Lerp( const Vector4 &v1, const Vector4 &v2, float percentage )
    {
        float oneMinusPercentage = 1.0f - percentage;

        return Vector4{ v1.x * oneMinusPercentage + v2.x * percentage,
            v1.y * oneMinusPercentage + v2.y * percentage,
            v1.z * oneMinusPercentage + v2.z * percentage,
            v1.w * oneMinusPercentage + v2.w * percentage };
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