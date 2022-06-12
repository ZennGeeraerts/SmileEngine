#pragma once

#include "math.h"

namespace smile::math
{
    struct Vector2 final
    {
        union
        {
            float m[2];
            struct
            {
                float x;
                float y;
            };
        };

        static const Vector2 zero, xAxis, yAxis, one;
    };

    inline float dotProduct( const Vector2 &v1, const Vector2 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline float distanceSqr( const Vector2 &v1, const Vector2 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y );
    }

    inline float distance( const Vector2 &v1, const Vector2 &v2 )
    {
        return squareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) );
    }

    inline float lengthSqr( const Vector2 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y;
    }

    inline float length( const Vector2 &vector )
    {
        return squareRoot( vector.x * vector.x + vector.y * vector.y );
    }

    inline bool isUnitVector( const Vector2 &vector )
    {
        return isOne( vector.x * vector.x + vector.y * vector.y );
    }

    inline bool hasZeroLength( const Vector2 &vector, float precision = epsilon )
    {
        return isSquareZero( vector.x * vector.x + vector.y * vector.y, precision );
    }

    inline bool isZeroVector( const Vector2 &vector, float precision = epsilon )
    {
        return isZero( vector.x, precision ) && isZero( vector.y, precision );
    }

    inline bool compareVector2( const Vector2 &v1, const Vector2 &v2, float precision = epsilon )
    {
        return compareFloats( v1.x, v2.x, precision ) && compareFloats( v1.y, v2.y, precision );
    }

    inline Vector2 getNormalized( const Vector2 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "getNormalized > Vector2 is zero vector" );

        const float length = squareRoot( vector.x * vector.x + vector.y * vector.y );
        return Vector2{ vector.x / length, vector.y / length };
    }

    inline void normalize( Vector2 &vector )
    {
        SM_ASSERT( !isZeroVector( vector ), "normalize > Vector2 is zero vector" );

        float length = squareRoot( vector.x * vector.x + vector.y * vector.y );
        vector.x /= length;
        vector.y /= length;
    }

    inline Vector2 lerp( const Vector2 &v1, const Vector2 &v2, float percentage )
    {
        const float one_minus_percentage = 1.0f - percentage;

        return Vector2{
            v1.x * one_minus_percentage + v2.x * percentage, v1.y * one_minus_percentage + v2.y * percentage };
    }

    inline Vector2 RotateVector( const Vector2 &vector, float angle )
    {
        const float cosinus_result = cosinus( angle );
        const float sinus_result = sinus( angle );

        return Vector2{
            vector.x * cosinus_result + vector.y * sinus_result, -vector.x * sinus_result + vector.y * cosinus_result };
    }

    inline float getAngle( const Vector2 &vector )
    {
        return arcTangent( vector.y, vector.x );
    }

    // Operators

    inline bool operator==( const Vector2 &lhs, const Vector2 &rhs )
    {
        return ( lhs.x == rhs.x ) && ( lhs.y == rhs.y );
    }

    inline bool operator!=( const Vector2 &lhs, const Vector2 &rhs )
    {
        return ( lhs.x != rhs.x ) || ( lhs.y != rhs.y );
    }

    inline Vector2 operator+( const Vector2 &lhs, const Vector2 &rhs )
    {
        return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
    }

    inline Vector2 &operator+=( Vector2 &lhs, const Vector2 &rhs )
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    inline Vector2 operator-( const Vector2 &lhs, const Vector2 &rhs )
    {
        return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
    }

    inline Vector2 &operator-=( Vector2 &lhs, const Vector2 &rhs )
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    inline Vector2 operator*( const Vector2 &lhs, const Vector2 &rhs )
    {
        return Vector2{ lhs.x * rhs.x, lhs.y * rhs.y };
    }

    inline Vector2 operator*( const Vector2 &lhs, float rhs )
    {
        return Vector2{ lhs.x * rhs, lhs.y * rhs };
    }

    inline Vector2 &operator*=( Vector2 &lhs, const Vector2 &rhs )
    {
        lhs.x *= rhs.x;
        lhs.y *= rhs.y;
        return lhs;
    }

    inline Vector2 &operator*=( Vector2 &lhs, float rhs )
    {
        lhs.x *= rhs;
        lhs.y *= rhs;
        return lhs;
    }

    inline Vector2 operator/( const Vector2 &lhs, const Vector2 &rhs )
    {
        return Vector2{ lhs.x / rhs.x, lhs.y / rhs.y };
    }

    inline Vector2 operator/( const Vector2 &lhs, float rhs )
    {
        return Vector2{ lhs.x / rhs, lhs.y / rhs };
    }

    inline Vector2 &operator/=( Vector2 &lhs, const Vector2 &rhs )
    {
        lhs.x /= rhs.x;
        lhs.y /= rhs.y;
        return lhs;
    }

    inline Vector2 &operator/=( Vector2 &lhs, float rhs )
    {
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
    }

    inline Vector2 operator-( const Vector2 &vector )
    {
        return Vector2{ -vector.x, -vector.y };
    }
}
