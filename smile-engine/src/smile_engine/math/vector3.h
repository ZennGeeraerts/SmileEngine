/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "math.h"

namespace smile::math
{
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

        static const Vector3 Zero, XAxis, YAxis, ZAxis, One;
    };

    inline float DotProduct( const Vector3 &v1, const Vector3 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline Vector3 CrossProduct( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    }

    inline float DistanceSqr( const Vector3 &v1, const Vector3 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z );
    }

    inline float Distance( const Vector3 &v1, const Vector3 &v2 )
    {
        return SquareRoot(
            ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
    }

    inline float LengthSqr( const Vector3 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
    }

    inline float Length( const Vector3 &vector )
    {
        return SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    inline bool IsUnitVector( const Vector3 &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    inline bool HasZeroLength( const Vector3 &vector, float precision = g_Epsilon )
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, precision );
    }

    inline bool IsZeroVector( const Vector3 &vector, float precision = g_Epsilon )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision );
    }

    inline Vector3 GetNormalized( const Vector3 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "GetNormalized > Vector3 is zero vector" );

        float length = SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
        return Vector3{ vector.x / length, vector.y / length, vector.z / length };
    }

    inline void Normalize( Vector3 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "Normalize > Vector3 is zero vector" );

        float length = SquareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
    }

    inline Vector3 Lerp( const Vector3 &v1, const Vector3 &v2, float percentage )
    {
        const float oneMinusPercentage = 1.0f - percentage;

        return Vector3{ v1.x * oneMinusPercentage + v2.x * percentage,
            v1.y * oneMinusPercentage + v2.y * percentage,
            v1.z * oneMinusPercentage + v2.z * percentage };
    }

    inline Vector3 GetPerpendicular( const Vector3 &vector )
    {
        auto perpendicular = CrossProduct( vector, Vector3::XAxis );

        if ( HasZeroLength( perpendicular ) )
        {
            perpendicular = CrossProduct( vector, Vector3::YAxis );
        }

        return GetNormalized( perpendicular );
    }

    inline Vector3 GenerateRandom( const Vector3 &minimum, const Vector3 &maximum )
    {
        return Vector3{ GenerateRandom( minimum.x, maximum.x ),
            GenerateRandom( minimum.y, maximum.y ),
            GenerateRandom( minimum.z, maximum.z ) };
    }

    inline Vector3 Minimum( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ Minimum( v1.x, v2.x ), Minimum( v1.y, v2.y ), Minimum( v1.z, v2.z ) };
    }

    inline Vector3 Maximum( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ Maximum( v1.x, v2.x ), Maximum( v1.y, v2.y ), Maximum( v1.z, v2.z ) };
    }

    // Operators

    inline bool operator==( const Vector3 &lhs, const Vector3 &rhs )
    {
        return ( lhs.x == rhs.x ) && ( lhs.y == rhs.y ) && ( lhs.z == rhs.z );
    }

    inline bool operator!=( const Vector3 &lhs, const Vector3 &rhs )
    {
        return ( lhs.x != rhs.x ) || ( lhs.y != rhs.y ) || ( lhs.z != rhs.z );
    }

    inline Vector3 operator+( const Vector3 &lhs, const Vector3 &rhs )
    {
        return Vector3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
    }

    inline Vector3 &operator+=( Vector3 &lhs, const Vector3 &rhs )
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    inline Vector3 operator-( const Vector3 &lhs, const Vector3 &rhs )
    {
        return Vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
    }

    inline Vector3 &operator-=( Vector3 &lhs, const Vector3 &rhs )
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    inline Vector3 operator*( const Vector3 &lhs, const Vector3 &rhs )
    {
        return Vector3{ lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
    }

    inline Vector3 operator*( const Vector3 &lhs, float rhs )
    {
        return Vector3{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
    }

    inline Vector3 &operator*=( Vector3 &lhs, const Vector3 &rhs )
    {
        lhs.x *= rhs.x;
        lhs.y *= rhs.y;
        lhs.z *= rhs.z;
        return lhs;
    }

    inline Vector3 &operator*=( Vector3 &lhs, float rhs )
    {
        lhs.x *= rhs;
        lhs.y *= rhs;
        lhs.z *= rhs;
        return lhs;
    }

    inline Vector3 operator/( const Vector3 &lhs, const Vector3 &rhs )
    {
        return Vector3{ lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z };
    }

    inline Vector3 operator/( const Vector3 &lhs, float rhs )
    {
        return Vector3{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
    }

    inline Vector3 &operator/=( Vector3 &lhs, const Vector3 &rhs )
    {
        lhs.x /= rhs.x;
        lhs.y /= rhs.y;
        lhs.z /= rhs.z;
        return lhs;
    }

    inline Vector3 &operator/=( Vector3 &lhs, float rhs )
    {
        lhs.x /= rhs;
        lhs.y /= rhs;
        lhs.z /= rhs;
        return lhs;
    }

    inline Vector3 operator-( const Vector3 &vector )
    {
        return Vector3{ -vector.x, -vector.y, -vector.z };
    }
}