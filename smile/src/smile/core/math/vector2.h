/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "math.h"
#include "vector.h"

namespace smile::math
{
    template < typename Type >
    struct Vector< 2, Type >
    {
        Vector< 2, Type >() = default;
        Vector< 2, Type >( Type x, Type y ) : x{ x }, y{ y }
        {
        }
        Vector< 2, Type >( const Vector< 2, Type > &vector ) : x{ vector.x }, y{ vector.y }
        {
        }
        Vector< 2, Type >( Vector< 2, Type > &&vector ) noexcept
            : x{ std::move( vector.x ) }, y{ std::move( vector.y ) }
        {
        }
        explicit Vector< 2, Type >( const Vector< 3, Type > &vector ) : x{ vector.x }, y{ vector.y }
        {
        }
        explicit Vector< 2, Type >( const Vector< 4, Type > &vector ) : x{ vector.x }, y{ vector.y }
        {
        }

        template < typename U >
        inline Vector< 2, Type > operator+( const Vector< 2, U > &vector ) const
        {
            return Vector< 2, Type >{ x + static_cast< Type >( vector.x ), y + static_cast< Type >( vector.y ) };
        }

        template < typename U >
        inline Vector< 2, Type > operator-( const Vector< 2, U > &vector ) const
        {
            return Vector< 2, Type >{ x - static_cast< Type >( vector.x ), y - static_cast< Type >( vector.y ) };
        }

        inline Vector< 2, Type > operator*( const Vector< 2, Type > &vector ) const
        {
            return Vector< 2, Type >{ x * vector.x, y * vector.y };
        }

        inline Vector< 2, Type > operator*( Type scale ) const
        {
            return Vector< 2, Type >{ x * scale, y * scale };
        }

        inline Vector< 2, Type > operator/( const Vector< 2, Type > &vector ) const
        {
            return Vector< 2, Type >{ x / vector.x, y / vector.y };
        }

        inline Vector< 2, Type > operator/( Type scale ) const
        {
            return Vector< 2, Type >{ x / scale, y / scale };
        }

        inline Vector< 2, Type > &operator=( const Vector< 2, Type > &vector )
        {
            x = vector.x;
            y = vector.y;
            return *this;
        }

        inline Vector< 2, Type > &operator+=( const Vector< 2, Type > &vector )
        {
            x += vector.x;
            y += vector.y;
            return *this;
        }

        inline Vector< 2, Type > &operator-=( const Vector< 2, Type > &vector )
        {
            x -= vector.x;
            y -= vector.y;
            return *this;
        }

        inline Vector< 2, Type > &operator*=( const Vector< 2, Type > &vector )
        {
            x *= vector.x;
            y *= vector.y;
            return *this;
        }

        inline Vector< 2, Type > &operator*=( Type scale )
        {
            x *= scale;
            y *= scale;
            return *this;
        }

        inline Vector< 2, Type > &operator/=( const Vector< 2, Type > &vector )
        {
            x /= vector.x;
            y /= vector.y;
            return *this;
        }

        inline Vector< 2, Type > &operator/=( Type scale )
        {
            x /= scale;
            y /= scale;
            return *this;
        }

        inline Vector< 2, Type > operator-() const
        {
            return Vector< 2, Type >{ -x, -y };
        }

        inline bool operator==( const Vector< 2, Type > &vector ) const
        {
            return AreEqual< Type >( x, vector.x ) && AreEqual< Type >( y, vector.y );
        }

        inline bool operator!=( const Vector< 2, Type > &vector ) const
        {
            return !( *this == vector );
        }

        inline Type operator[]( Uint8 index ) const
        {
            SM_ASSERT_MSG( index < 2, "Vector2 > index of Vector2 [] operator is out of bounds!" );
            return Data[index];
        }

        inline Type &operator[]( Uint8 index )
        {
            SM_ASSERT_MSG( index < 2, "Vector2 > index of Vector2 [] operator is out of bounds!" );
            return Data[index];
        }

        static constexpr Vector< 2, Type > ZeroVector();
        static constexpr Vector< 2, Type > XAxis();
        static constexpr Vector< 2, Type > YAxis();
        static constexpr Vector< 2, Type > OneVector();

        union
        {
            Type Data[2];
            struct
            {
                Type x, y;
            };
            struct
            {
                Type r, g;
            };
        };
    };

    template < typename Type, typename U >
    inline Vector< 2, Type > operator*( const Vector< 2, Type > &vector, U scale )
    {
        Type s = static_cast< Type >( scale );
        return Vector< 2, Type >( vector.x * s, vector.y * s );
    }

    template < typename Type >  
    inline constexpr Vector< 2, Type > Vector< 2, Type >::ZeroVector()
    {
        Type zero = static_cast< Type >( 0 );
        return Vector< 2, Type >( zero, zero );
    }

    template < typename Type >  
    inline constexpr Vector< 2, Type > Vector< 2, Type >::XAxis()
    {
        Type zero = static_cast< Type >( 0 );
        Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >( one, zero );
    }

    template < typename Type >  
    inline constexpr Vector< 2, Type > Vector< 2, Type >::YAxis()
    {
        Type zero = static_cast< Type >( 0 );
        Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >( zero, one );
    }

    template < typename Type >  
    inline constexpr Vector< 2, Type > Vector< 2, Type >::OneVector()
    {
        Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >( one, one );
    }

    template < typename Type >
    inline Type DotProduct( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template < typename Type >
    inline Type CrossProduct( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 )
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    template < typename Type >
    inline Type DistanceSqr( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y );
    }

    template < typename Type >
    inline Type Distance( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 )
    {
        return SquareRoot( DistanceSqr( v1, v2 ) );
    }

    //inline bool IsUnitVector( const Vector2 &vector )
    //{
    //    return IsOne( vector.x * vector.x + vector.y * vector.y );
    //}

    //inline bool HasZeroLength( const Vector2 &vector, float precision = g_Epsilon )
    //{
    //    return IsSquareZero( vector.x * vector.x + vector.y * vector.y, precision );
    //}

    //inline bool IsZeroVector( const Vector2 &vector, float precision = g_Epsilon )
    //{
    //    return IsZero( vector.x, precision ) && IsZero( vector.y, precision );
    //}

    //inline Vector2 GetNormalized( const Vector2 &vector )
    //{
    //    SM_ASSERT( !IsZeroVector( vector ), "GetNormalized > Vector2 is zero vector" );

    //    const float length = SquareRoot( vector.x * vector.x + vector.y * vector.y );
    //    return Vector2{ vector.x / length, vector.y / length };
    //}

    //inline void Normalize( Vector2 &vector )
    //{
    //    SM_ASSERT( !IsZeroVector( vector ), "Normalize > Vector2 is zero vector" );

    //    float length = SquareRoot( vector.x * vector.x + vector.y * vector.y );
    //    vector.x /= length;
    //    vector.y /= length;
    //}

    //inline Vector2 Lerp( const Vector2 &v1, const Vector2 &v2, float percentage )
    //{
    //    const float oneMinusPercentage = 1.0f - percentage;

    //    return Vector2{
    //        v1.x * oneMinusPercentage + v2.x * percentage, v1.y * oneMinusPercentage + v2.y * percentage };
    //}

    //inline Vector2 RotateVector( const Vector2 &vector, float angle )
    //{
    //    const float cosinusResult = Cosinus( angle );
    //    const float sinusResult = Sinus( angle );

    //    return Vector2{
    //        vector.x * cosinusResult + vector.y * sinusResult, -vector.x * sinusResult + vector.y * cosinusResult };
    //}

    //inline float GetAngle( const Vector2 &vector )
    //{
    //    return ArcTangent( vector.y, vector.x );
    //}

    //// Operators

    //inline bool operator==( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return ( lhs.x == rhs.x ) && ( lhs.y == rhs.y );
    //}

    //inline bool operator!=( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return ( lhs.x != rhs.x ) || ( lhs.y != rhs.y );
    //}

    //inline Vector2 operator+( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
    //}

    //inline Vector2 &operator+=( Vector2 &lhs, const Vector2 &rhs )
    //{
    //    lhs.x += rhs.x;
    //    lhs.y += rhs.y;
    //    return lhs;
    //}

    //inline Vector2 operator-( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
    //}

    //inline Vector2 &operator-=( Vector2 &lhs, const Vector2 &rhs )
    //{
    //    lhs.x -= rhs.x;
    //    lhs.y -= rhs.y;
    //    return lhs;
    //}

    //inline Vector2 operator*( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return Vector2{ lhs.x * rhs.x, lhs.y * rhs.y };
    //}

    //inline Vector2 operator*( const Vector2 &lhs, float rhs )
    //{
    //    return Vector2{ lhs.x * rhs, lhs.y * rhs };
    //}

    //inline Vector2 &operator*=( Vector2 &lhs, const Vector2 &rhs )
    //{
    //    lhs.x *= rhs.x;
    //    lhs.y *= rhs.y;
    //    return lhs;
    //}

    //inline Vector2 &operator*=( Vector2 &lhs, float rhs )
    //{
    //    lhs.x *= rhs;
    //    lhs.y *= rhs;
    //    return lhs;
    //}

    //inline Vector2 operator/( const Vector2 &lhs, const Vector2 &rhs )
    //{
    //    return Vector2{ lhs.x / rhs.x, lhs.y / rhs.y };
    //}

    //inline Vector2 operator/( const Vector2 &lhs, float rhs )
    //{
    //    return Vector2{ lhs.x / rhs, lhs.y / rhs };
    //}

    //inline Vector2 &operator/=( Vector2 &lhs, const Vector2 &rhs )
    //{
    //    lhs.x /= rhs.x;
    //    lhs.y /= rhs.y;
    //    return lhs;
    //}

    //inline Vector2 &operator/=( Vector2 &lhs, float rhs )
    //{
    //    lhs.x /= rhs;
    //    lhs.y /= rhs;
    //    return lhs;
    //}

    //inline Vector2 operator-( const Vector2 &vector )
    //{
    //    return Vector2{ -vector.x, -vector.y };
    //}
}
