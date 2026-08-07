/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "vector.h"

namespace smile::math
{
    template < Numeric Type >
    class Vector< 4, Type > final
    {
      public:
        constexpr Vector< 4, Type >() noexcept = default;

        constexpr Vector< 4, Type >( Type x, Type y, Type z, Type w ) noexcept : x{ x }, y{ y }, z{ z }, w{ w }
        {
        }

        static constexpr Vector< 4, Type > ZeroVector() noexcept;
        static constexpr Vector< 4, Type > XAxis() noexcept;
        static constexpr Vector< 4, Type > YAxis() noexcept;
        static constexpr Vector< 4, Type > ZAxis() noexcept;
        static constexpr Vector< 4, Type > WAxis() noexcept;
        static constexpr Vector< 4, Type > OneVector() noexcept;

        union
        {
            Type Data[4];
            struct
            {
                Type x, y, z, w;
            };
            struct
            {
                Type r, g, b, a;
            };
        };
    };

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator+( const Vector< 4, T > &first, const Vector< 4, U > &second ) noexcept
    {
        return Vector< 4, T >{ first.x + static_cast< T >( second.x ),
            first.y + static_cast< T >( second.y ),
            first.z + static_cast< T >( second.z ),
            first.w + static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator-( const Vector< 4, T > &first, const Vector< 4, U > &second ) noexcept
    {
        return Vector< 4, T >{ first.x - static_cast< T >( second.x ),
            first.y - static_cast< T >( second.y ),
            first.z - static_cast< T >( second.z ),
            first.w - static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator*( const Vector< 4, T > &first, const Vector< 4, U > &second ) noexcept
    {
        return Vector< 4, T >{ first.x * static_cast< T >( second.x ),
            first.y * static_cast< T >( second.y ),
            first.z * static_cast< T >( second.z ),
            first.w * static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator*( const Vector< 4, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x * s, vector.y * s, vector.z * s, vector.w * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator*( const U scale, const Vector< 4, T > &vector ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x * s, vector.y * s, vector.z * s, vector.w * s };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator/( const Vector< 4, T > &first, const Vector< 4, U > &second ) noexcept
    {
        return Vector< 4, T >{ first.x / static_cast< T >( second.x ),
            first.y / static_cast< T >( second.y ),
            first.z / static_cast< T >( second.z ),
            first.w / static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > operator/( const Vector< 4, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x / s, vector.y / s, vector.z / s, vector.w / s };
    }

    template < Numeric T >
    constexpr Vector< 4, T > &operator+=( Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        first.x += second.x;
        first.y += second.y;
        first.z += second.z;
        first.w += second.w;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 4, T > &operator-=( Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        first.x -= second.x;
        first.y -= second.y;
        first.z -= second.z;
        first.w -= second.w;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 4, T > &operator*=( Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        first.x *= second.x;
        first.y *= second.y;
        first.z *= second.z;
        first.w *= second.w;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > &operator*=( Vector< 4, T > &vector, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        vector.x *= s;
        vector.y *= s;
        vector.z *= s;
        vector.w *= s;
        return vector;
    }

    template < Numeric T >
    constexpr Vector< 4, T > &operator/=( Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        first.x /= second.x;
        first.y /= second.y;
        first.z /= second.z;
        first.w /= second.w;
        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Vector< 4, T > &operator/=( Vector< 4, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );
        first.x /= s;
        first.y /= s;
        first.z /= s;
        first.w /= s;
        return first;
    }

    template < Numeric T >
    constexpr Vector< 4, T > operator-( const Vector< 4, T > &vector ) noexcept
    {
        return Vector< 4, T >{ -vector.x, -vector.y, -vector.z, -vector.w };
    }

    template < Numeric T >
    inline bool operator==( const Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        return AreEqual< T >( first.x, second.x ) && AreEqual< T >( first.y, second.y ) &&
               AreEqual< T >( first.z, second.z ) && AreEqual< T >( first.w, second.w );
    }

    template < Numeric T >
    inline bool operator!=( const Vector< 4, T > &first, const Vector< 4, T > &second ) noexcept
    {
        return !( first == second );
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::ZeroVector() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        return Vector< 4, Type >{ zero, zero, zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::XAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, zero, zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::YAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, one, zero, zero };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::ZAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, zero, one, zero };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::WAxis() noexcept
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, zero, zero, one };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > Vector< 4, Type >::OneVector() noexcept
    {
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ one, one, one, one };
    }

    template < Numeric Type >
    constexpr float DotProduct( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 ) noexcept
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template < Numeric Type >
    constexpr float DistanceSqr( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 ) noexcept
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w );
    }

    template < Numeric Type >
    inline float Distance( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 ) noexcept
    {
        return SquareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
                           ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w ) );
    }

    template < Numeric Type >
    constexpr bool IsUnitVector( const Vector< 4, Type > &vector ) noexcept
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    template < Numeric Type >
    constexpr bool HasZeroLength( const Vector< 4, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsSquareZero(
            vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w, precision );
    }

    template < Numeric Type >
    constexpr bool IsZeroVector( const Vector< 4, Type > &vector, float precision = g_Epsilon ) noexcept
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision ) &&
               IsZero( vector.w, precision );
    }
}