/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "math.h"
#include "vector.h"

namespace smile::math
{
    template < typename Type >
    class Vector< 2, Type >
    {
      public:
        Vector< 2, Type >() noexcept = default;

        Vector< 2, Type >( Type x, Type y ) noexcept : x{ x }, y{ y }
        {
        }

        Vector< 2, Type >( const Vector< 2, Type > &vector ) noexcept : x{ vector.x }, y{ vector.y }
        {
        }

        Vector< 2, Type >( Vector< 2, Type > &&vector ) noexcept
            : x{ std::move( vector.x ) }, y{ std::move( vector.y ) }
        {
        }

        explicit Vector< 2, Type >( const Vector< 3, Type > &vector ) noexcept : x{ vector.x }, y{ vector.y }
        {
        }

        explicit Vector< 2, Type >( const Vector< 4, Type > &vector ) noexcept : x{ vector.x }, y{ vector.y }
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
    inline Vector< 2, Type > operator*( U scale, const Vector< 2, Type > &vector )
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

    template < Uint8 Size, typename Type >
    inline bool IsZeroVector( const Vector< 2, Type > &vector, float precision = g_Epsilon )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision );
    }

    template < Uint8 Size, typename Type >
    inline bool IsUnitVector( const Vector< 2, Type > &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y );
    }

    template < Uint8 Size, typename Type >
    inline bool HasZeroLength( const Vector< 2, Type > &vector, float precision = g_Epsilon )
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y, precision );
    }

    template < Uint8 Size, typename Type >
    inline Vector< 2, Type > RotateVector( const Vector< 2, Type > &vector, float angle )
    {
        const float cosinusResult = Cosinus( angle );
        const float sinusResult = Sinus( angle );

        return Vector< 2, Type >{
            vector.x * cosinusResult + vector.y * sinusResult, -vector.x * sinusResult + vector.y * cosinusResult };
    }

    template < Uint8 Size, typename Type >
    inline float GetAngle( const Vector< 2, Type > &vector )
    {
        return ArcTangent( vector.y, vector.x );
    }
}
