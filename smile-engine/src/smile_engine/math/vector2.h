#pragma once

#include "math.h"

namespace smile::math
{
    struct Vector2 final
    {
        union
        {
            float M[2];
            struct
            {
                float x;
                float y;
            };
        };

        static const Vector2 Zero, XAxis, YAxis, One;
    };

    inline float DotProduct( const Vector2 &v1, const Vector2 &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    inline float DistanceSqr( const Vector2 &v1, const Vector2 &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y );
    }

    inline float Distance( const Vector2 &v1, const Vector2 &v2 )
    {
        return SquareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) );
    }

    inline float LengthSqr( const Vector2 &vector )
    {
        return vector.x * vector.x + vector.y * vector.y;
    }

    inline float Length( const Vector2 &vector )
    {
        return SquareRoot( vector.x * vector.x + vector.y * vector.y );
    }

    inline bool IsUnitVector( const Vector2 &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y );
    }

    inline bool HasZeroLength( const Vector2 &vector, float precision = g_EPSILON )
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y, precision );
    }

    inline bool IsZeroVector( const Vector2 &vector, float precision = g_EPSILON )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision );
    }

    inline bool CompareVector2( const Vector2 &v1, const Vector2 &v2, float precision = g_EPSILON )
    {
        return CompareFloats( v1.x, v2.x, precision ) && CompareFloats( v1.y, v2.y, precision );
    }

    inline Vector2 GetNormalized( const Vector2 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "GetNormalized > Vector2 is zero vector" );

        float length = SquareRoot( vector.x * vector.x + vector.y * vector.y );
        return Vector2{ vector.x / length, vector.y / length };
    }

    inline void Normalize( Vector2 &vector )
    {
        SM_ASSERT( !IsZeroVector( vector ), "Normalize > Vector2 is zero vector" );

        float length = SquareRoot( vector.x * vector.x + vector.y * vector.y );
        vector.x /= length;
        vector.y /= length;
    }

    inline Vector2 Lerp( const Vector2 &v1, const Vector2 &v2, float percentage )
    {
        float oneMinusPercentage = 1.0f - percentage;

        return Vector2{ v1.x * oneMinusPercentage + v2.x * percentage, v1.y * oneMinusPercentage + v2.y * percentage };
    }

    inline Vector2 RotateVector( const Vector2 &vector, float angle )
    {
        float cosinus = Cosinus( angle );
        float sinus = Sinus( angle );

        return Vector2{ vector.x * cosinus + vector.y * sinus, -vector.x * sinus + vector.y * cosinus };
    }

    inline float GetAngle( const Vector2 &vector )
    {
        return ArcTangent( vector.y, vector.x );
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
