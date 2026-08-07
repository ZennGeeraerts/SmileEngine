/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "vector.h"
#include "angle.h"

namespace smile::math
{
    template < Numeric Type >
    class Vector< 2, Type > final
    {
      public:
        constexpr Vector< 2, Type >() noexcept = default;

        constexpr Vector< 2, Type >( Type x, Type y ) noexcept : x{ x }, y{ y }
        {
        }

        explicit constexpr Vector< 2, Type >( const Vector< 3, Type > &other ) noexcept : x{ other.x }, y{ other.y }
        {
        }

        explicit constexpr Vector< 2, Type >( const Vector< 4, Type > &other ) noexcept : x{ other.x }, y{ other.y }
        {
        }

        static constexpr Vector< 2, Type > ZeroVector() noexcept;
        static constexpr Vector< 2, Type > XAxis() noexcept;
        static constexpr Vector< 2, Type > YAxis() noexcept;
        static constexpr Vector< 2, Type > OneVector() noexcept;

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

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator+( const Vector< 2, T > &first, const Vector< 2, U > &second ) noexcept
    {
        return Vector< 2, T >{ first.x + static_cast< T >( second.x ), first.y + static_cast< T >( second.y ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator-( const Vector< 2, T > &first, const Vector< 2, U > &second ) noexcept
    {
        return Vector< 2, T >{ first.x - static_cast< T >( second.x ), first.y - static_cast< T >( second.y ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator*( const Vector< 2, T > &first, const Vector< 2, U > &second ) noexcept
    {
        return Vector< 2, T >{ first.x * static_cast< T >( second.x ), first.y * static_cast< T >( second.y ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator*( const Vector< 2, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 2, T >{ vector.x * s, vector.y * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator*( const U scale, const Vector< 2, T > &vector ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 2, T >{ vector.x * s, vector.y * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator/( const Vector< 2, T > &first, const Vector< 2, U > &second ) noexcept
    {
        return Vector< 2, T >{ first.x / static_cast< T >( second.x ), first.y / static_cast< T >( second.y ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > operator/( const Vector< 2, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 2, T >{ vector.x / s, vector.y / s };
    }

    template < Numeric T >
    constexpr Vector< 2, T > &operator+=( Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        first.x += second.x;
        first.y += second.y;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 2, T > &operator-=( Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        first.x -= second.x;
        first.y -= second.y;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 2, T > &operator*=( Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        first.x *= second.x;
        first.y *= second.y;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > &operator*=( Vector< 2, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        vector.x *= s;
        vector.y *= s;
        return vector;
    }

    template < Numeric T >
    constexpr Vector< 2, T > &operator/=( Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        first.x /= second.x;
        first.y /= second.y;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 2, T > &operator/=( Vector< 2, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        first.x /= s;
        first.y /= s;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 2, T > operator-( const Vector< 2, T > &vector ) noexcept
    {
        return Vector< 2, T >{ -vector.x, -vector.y };
    }

    template < Numeric T >
    inline bool operator==( const Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        return AreEqual< T >( first.x, second.x ) && AreEqual< T >( first.y, second.y );
    }

    template < Numeric T >
    inline bool operator!=( const Vector< 2, T > &first, const Vector< 2, T > &second ) noexcept
    {
        return !( first == second );
    }

    template < Numeric Type >
    constexpr Vector< 2, Type > Vector< 2, Type >::ZeroVector() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        return Vector< 2, Type >{ zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 2, Type > Vector< 2, Type >::XAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >{ one, zero };
    }

    template < Numeric Type >
    constexpr Vector< 2, Type > Vector< 2, Type >::YAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >{ zero, one };
    }

    template < Numeric Type >
    constexpr Vector< 2, Type > Vector< 2, Type >::OneVector() noexcept
    {
        const Type one = static_cast< Type >( 1 );
        return Vector< 2, Type >{ one, one };
    }

    template < Numeric Type >
    constexpr Type DotProduct( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 ) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template < Numeric Type >
    constexpr Type CrossProduct( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 ) noexcept
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    template < Numeric Type >
    constexpr Type DistanceSqr( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 ) noexcept
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y );
    }

    template < Numeric Type >
    inline Type Distance( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 ) noexcept
    {
        return SquareRoot( DistanceSqr( v1, v2 ) );
    }

    template < Uint8 Size, Numeric Type >
    constexpr bool IsZeroVector( const Vector< 2, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision );
    }

    template < Uint8 Size, Numeric Type >
    constexpr bool IsUnitVector( const Vector< 2, Type > &vector ) noexcept
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y );
    }

    template < Uint8 Size, Numeric Type >
    constexpr bool HasZeroLength( const Vector< 2, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y, precision );
    }

    template < Uint8 Size, Numeric Type >
    inline Vector< 2, Type > RotateVector( const Vector< 2, Type > &vector, const Angle angle ) noexcept
    {
        const float cosinusResult = Cosinus( angle );
        const float sinusResult = Sinus( angle );

        return Vector< 2, Type >{
            vector.x * cosinusResult + vector.y * sinusResult, -vector.x * sinusResult + vector.y * cosinusResult };
    }

    template < Uint8 Size, Numeric Type >
    inline Angle GetAngle( const Vector< 2, Type > &vector ) noexcept
    {
        return ArcTangent( vector.y, vector.x );
    }
}
