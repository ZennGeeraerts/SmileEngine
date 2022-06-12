#pragma once

#include "math.h"

namespace smile::math
{
    struct Vector3 final
    {
        union
        {
            float m[3];
            struct
            {
                float x;
                float y;
                float z;
            };
        };

        static const Vector3 zero, xAxis, yAxis, zAxis, one;
    };

    inline float dotProduct( const Vector3 &v1, const Vector3 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline Vector3 crossProduct( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    }

    inline float distanceSqr( const Vector3 &v1, const Vector3 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z );
    }

    inline float distance( const Vector3 &v1, const Vector3 &v2 )
    {
        return squareRoot(
            ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
    }

    inline float lengthSqr( const Vector3 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
    }

    inline float length( const Vector3 &vector )
    {
        return squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    inline bool isUnitVector( const Vector3 &vector )
    {
        return isOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    inline bool hasZeroLength( const Vector3 &vector, float precision = epsilon )
    {
        return isSquareZero( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, precision );
    }

    inline bool isZeroVector( const Vector3 &vector, float precision = epsilon )
    {
        return isZero( vector.x, precision ) && isZero( vector.y, precision ) && isZero( vector.z, precision );
    }

    inline bool compareVector3( const Vector3 &v1, const Vector3 &v2, float precision = epsilon )
    {
        return compareFloats( v1.x, v2.x, precision ) && compareFloats( v1.y, v2.y, precision ) &&
               compareFloats( v1.z, v2.z, precision );
    }

    inline Vector3 getNormalized( const Vector3 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "getNormalized > Vector3 is zero vector" );

        float length = squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
        return Vector3{ vector.x / length, vector.y / length, vector.z / length };
    }

    inline void normalize( Vector3 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "Normalize > Vector3 is zero vector" );

        float length = squareRoot( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
    }

    inline Vector3 lerp( const Vector3 &v1, const Vector3 &v2, float percentage )
    {
        const float one_minus_percentage = 1.0f - percentage;

        return Vector3{ v1.x * one_minus_percentage + v2.x * percentage,
            v1.y * one_minus_percentage + v2.y * percentage,
            v1.z * one_minus_percentage + v2.z * percentage };
    }

    inline Vector3 getPerpendicular( const Vector3 &vector )
    {
        auto perpendicular = crossProduct( vector, Vector3::xAxis );

        if ( hasZeroLength( perpendicular ) )
        {
            perpendicular = crossProduct( vector, Vector3::yAxis );
        }

        return getNormalized( perpendicular );
    }

    inline Vector3 generateRandom( const Vector3 &minimum, const Vector3 &maximum )
    {
        return Vector3{ generateRandom( minimum.x, maximum.x ),
            generateRandom( minimum.y, maximum.y ),
            generateRandom( minimum.z, maximum.z ) };
    }

    inline Vector3 minimum( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ minimum( v1.x, v2.x ), minimum( v1.y, v2.y ), minimum( v1.z, v2.z ) };
    }

    inline Vector3 maximum( const Vector3 &v1, const Vector3 &v2 )
    {
        return Vector3{ maximum( v1.x, v2.x ), maximum( v1.y, v2.y ), maximum( v1.z, v2.z ) };
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