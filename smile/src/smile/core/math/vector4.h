/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "vector.h"
#include "math_utilities.h"

namespace smile::math
{
    template < Numeric Type >
    class Vector< 4, Type > final
    {
      public:
        Vector< 4, Type >() noexcept = default;

        Vector< 4, Type >( Type x, Type y, Type z, Type w ) noexcept : x{ x }, y{ y }, z{ z }, w{ w }
        {
        }

        Vector< 4, Type >( const Vector< 4, Type > &other ) noexcept
            : x{ other.x }, y{ other.y }, z{ other.z }, w{ other.w }
        {
        }

        Vector< 4, Type >( Vector< 3, Type > &&other ) noexcept
            : x{ std::move( other.x ) }, y{ std::move( other.y ) }, z{ std::move( other.z ) }, w{ std::move( other.w ) }
        {
        }

        inline Vector< 4, Type > &operator=( const Vector< 4, Type > &other ) noexcept
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        static constexpr Vector< 4, Type > ZeroVector();
        static constexpr Vector< 4, Type > XAxis();
        static constexpr Vector< 4, Type > YAxis();
        static constexpr Vector< 4, Type > ZAxis();
        static constexpr Vector< 4, Type > WAxis();
        static constexpr Vector< 4, Type > OneVector();

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
    inline Vector< 4, T > operator+( const Vector< 4, T > &first, const Vector< 4, U > &second )
    {
        return Vector< 4, T >{ first.x + static_cast< T >( second.x ),
            first.y + static_cast< T >( second.y ),
            first.z + static_cast< T >( second.z ),
            first.w + static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator-( const Vector< 4, T > &first, const Vector< 4, U > &second )
    {
        return Vector< 4, T >{ first.x - static_cast< T >( second.x ),
            first.y - static_cast< T >( second.y ),
            first.z - static_cast< T >( second.z ),
            first.w - static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator*( const Vector< 4, T > &first, const Vector< 4, U > &second )
    {
        return Vector< 4, T >{ first.x * static_cast< T >( second.x ),
            first.y * static_cast< T >( second.y ),
            first.z * static_cast< T >( second.z ),
            first.w * static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator*( const Vector< 4, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x * s, vector.y * s, vector.z * s, vector.w * s };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator*( const U scale, const Vector< 4, T > &vector )
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x * s, vector.y * s, vector.z * s, vector.w * s };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator/( const Vector< 4, T > &first, const Vector< 4, U > &second )
    {
        return Vector< 4, T >{ first.x / static_cast< T >( second.x ),
            first.y / static_cast< T >( second.y ),
            first.z / static_cast< T >( second.z ),
            first.w / static_cast< T >( second.w ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > operator/( const Vector< 4, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        return Vector< 4, T >{ vector.x / s, vector.y / s, vector.z / s, vector.w / s };
    }

    template < Numeric T >
    inline Vector< 4, T > &operator+=( Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        first.x += second.x;
        first.y += second.y;
        first.z += second.z;
        first.w += second.w;
        return first;
    }

    template < Numeric T >
    inline Vector< 4, T > &operator-=( Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        first.x -= second.x;
        first.y -= second.y;
        first.z -= second.z;
        first.w -= second.w;
        return first;
    }

    template < Numeric T >
    inline Vector< 4, T > &operator*=( Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        first.x *= second.x;
        first.y *= second.y;
        first.z *= second.z;
        first.w *= second.w;
        return first;
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > &operator*=( Vector< 4, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        vector.x *= s;
        vector.y *= s;
        vector.z *= s;
        vector.w *= s;
        return vector;
    }

    template < Numeric T >
    inline Vector< 4, T > &operator/=( Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        first.x /= second.x;
        first.y /= second.y;
        first.z /= second.z;
        first.w /= second.w;
        return first;
    }

    template < Numeric T, Numeric U >
    inline Vector< 4, T > &operator/=( Vector< 4, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );
        first.x /= s;
        first.y /= s;
        first.z /= s;
        first.w /= s;
        return first;
    }

    template < Numeric T >
    inline Vector< 4, T > operator-( const Vector< 4, T > &vector )
    {
        return Vector< 4, T >{ -vector.x, -vector.y, -vector.z, -vector.w };
    }

    template < Numeric T >
    inline bool operator==( const Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        return AreEqual< T >( first.x, second.x ) && AreEqual< T >( first.y, second.y ) &&
               AreEqual< T >( first.z, second.z ) && AreEqual< T >( first.w, second.w );
    }

    template < Numeric T >
    inline bool operator!=( const Vector< 4, T > &first, const Vector< 4, T > &second )
    {
        return !( first == second );
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::ZeroVector()
    {
        const Type zero = static_cast< Type >( 0 );
        return Vector< 4, Type >{ zero, zero, zero, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::XAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, zero, zero, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::YAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, one, zero, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::ZAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, zero, one, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::WAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ zero, zero, zero, one };
    }

    template < Numeric Type >
    inline constexpr Vector< 4, Type > Vector< 4, Type >::OneVector()
    {
        const Type one = static_cast< Type >( 1 );
        return Vector< 4, Type >{ one, one, one, one };
    }

    template < Numeric Type >
    inline float DotProduct( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    template < Numeric Type >
    inline float DistanceSqr( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w );
    }

    template < Numeric Type >
    inline float Distance( const Vector< 4, Type > &v1, const Vector< 4, Type > &v2 )
    {
        return SquareRoot( ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
                           ( v1.z - v2.z ) * ( v1.z - v2.z ) + ( v1.w - v2.w ) * ( v1.w - v2.w ) );
    }

    template < Numeric Type >
    inline bool IsUnitVector( const Vector< 4, Type > &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w );
    }

    template < Numeric Type >
    inline bool HasZeroLength( const Vector< 4, Type > &vector, float precision = g_Epsilon )
    {
        return IsSquareZero(
            vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w, precision );
    }

    template < Numeric Type >
    inline bool IsZeroVector( const Vector< 4, Type > &vector, float precision = g_Epsilon )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision ) &&
               IsZero( vector.w, precision );
    }
}