/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "vector.h"

namespace smile::math
{
    template < Numeric Type >
    class Vector< 3, Type > final
    {
      public:
        constexpr Vector< 3, Type >() noexcept = default;

        constexpr Vector< 3, Type >( Type x, Type y, Type z ) noexcept : x{ x }, y{ y }, z{ z }
        {
        }

        explicit constexpr Vector< 3, Type >( const Vector< 4, Type > &other ) noexcept
            : x{ other.x }, y{ other.y }, z{ other.z }
        {
        }

        static constexpr Vector< 3, Type > ZeroVector() noexcept;
        static constexpr Vector< 3, Type > XAxis() noexcept;
        static constexpr Vector< 3, Type > YAxis() noexcept;
        static constexpr Vector< 3, Type > ZAxis() noexcept;
        static constexpr Vector< 3, Type > OneVector() noexcept;

        union
        {
            Type Data[3];
            struct
            {
                Type x, y, z;
            };
            struct
            {
                Type r, g, b;
            };
        };
    };

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator+( const Vector< 3, T > &first, const Vector< 3, U > &second ) noexcept
    {
        return Vector< 3, T >{ first.x + static_cast< T >( second.x ),
            first.y + static_cast< T >( second.y ),
            first.z + static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator-( const Vector< 3, T > &first, const Vector< 3, U > &second ) noexcept
    {
        return Vector< 3, T >{ first.x - static_cast< T >( second.x ),
            first.y - static_cast< T >( second.y ),
            first.z - static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator*( const Vector< 3, T > &first, const Vector< 3, U > &second ) noexcept
    {
        return Vector< 3, T >{ first.x * static_cast< T >( second.x ),
            first.y * static_cast< T >( second.y ),
            first.z * static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator*( const Vector< 3, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x * s, vector.y * s, vector.z * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator*( const U scale, const Vector< 3, T > &vector ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x * s, vector.y * s, vector.z * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator/( const Vector< 3, T > &first, const Vector< 3, U > &second ) noexcept
    {
        return Vector< 3, T >{ first.x / static_cast< T >( second.x ),
            first.y / static_cast< T >( second.y ),
            first.z / static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > operator/( const Vector< 3, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x / s, vector.y / s, vector.z / s };
    }

    template < Numeric T >
    constexpr Vector< 3, T > &operator+=( Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        first.x += second.x;
        first.y += second.y;
        first.z += second.z;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 3, T > &operator-=( Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        first.x -= second.x;
        first.y -= second.y;
        first.z -= second.z;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 3, T > &operator*=( Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        first.x *= second.x;
        first.y *= second.y;
        first.z *= second.z;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > &operator*=( Vector< 3, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        vector.x *= s;
        vector.y *= s;
        vector.z *= s;
        return vector;
    }

    template < Numeric T >
    constexpr Vector< 3, T > &operator/=( Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        first.x /= second.x;
        first.y /= second.y;
        first.z /= second.z;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 3, T > &operator/=( Vector< 3, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        first.x /= s;
        first.y /= s;
        first.z /= s;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 3, T > operator-( const Vector< 3, T > &vector ) noexcept
    {
        return Vector< 3, T >{ -vector.x, -vector.y, -vector.z };
    }

    template < Numeric T >
    inline bool operator==( const Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        return AreEqual< T >( first.x, second.x ) && AreEqual< T >( first.y, second.y ) &&
               AreEqual< T >( first.z, second.z );
    }

    template < Numeric T >
    inline bool operator!=( const Vector< 3, T > &first, const Vector< 3, T > &second ) noexcept
    {
        return !( first == second );
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Vector< 3, Type >::ZeroVector() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        return Vector< 3, Type >{ zero, zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Vector< 3, Type >::XAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Vector< 3, Type >::YAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ zero, one, zero };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Vector< 3, Type >::ZAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ zero, zero, one };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Vector< 3, Type >::OneVector() noexcept
    {
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, one, one };
    }

    template < Numeric Type >
    constexpr Type DotProduct( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > CrossProduct( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return Vector< 3, Type >{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    }

    template < Numeric Type >
    constexpr Type DistanceSqr( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z );
    }

    template < Numeric Type >
    inline Type Distance( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return SquareRoot(
            ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
    }

    template < Numeric Type >
    constexpr bool IsUnitVector( const Vector< 3, Type > &vector ) noexcept
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    template < Numeric Type >
    constexpr bool HasZeroLength( const Vector< 3, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, precision );
    }

    template < Numeric Type >
    constexpr bool IsZeroVector( const Vector< 3, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision );
    }

    template < Numeric Type >
    inline Vector< 3, Type > GetPerpendicular( const Vector< 3, Type > &vector ) noexcept
    {
        auto perpendicular = CrossProduct( vector, Vector< 3, Type >::XAxis() );

        if ( HasZeroLength( perpendicular ) )
        {
            perpendicular = CrossProduct( vector, Vector< 3, Type >::YAxis() );
        }

        return GetNormalized( perpendicular );
    }

    template < Numeric Type >
    inline Vector< 3, Type > GenerateRandom( const Vector< 3, Type > &minimum,
        const Vector< 3, Type > &maximum ) noexcept
    {
        return Vector< 3, Type >{ GenerateRandom( minimum.x, maximum.x ),
            GenerateRandom( minimum.y, maximum.y ),
            GenerateRandom( minimum.z, maximum.z ) };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Minimum( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return Vector< 3, Type >{ Minimum( v1.x, v2.x ), Minimum( v1.y, v2.y ), Minimum( v1.z, v2.z ) };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > Maximum( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 ) noexcept
    {
        return Vector< 3, Type >{ Maximum( v1.x, v2.x ), Maximum( v1.y, v2.y ), Maximum( v1.z, v2.z ) };
    }
}