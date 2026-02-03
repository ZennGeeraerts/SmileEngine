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
        Vector< 3, Type >() noexcept = default;

        Vector< 3, Type >( Type x, Type y, Type z ) noexcept : x{ x }, y{ y }, z{ z }
        {
        }

        Vector< 3, Type >( const Vector< 3, Type > &other ) noexcept : x{ other.x }, y{ other.y }, z{ other.z }
        {
        }

        Vector< 3, Type >( Vector< 3, Type > &&other ) noexcept
            : x{ std::move( other.x ) }, y{ std::move( other.y ) }, z{ std::move( other.z ) }
        {
        }

        explicit Vector< 3, Type >( const Vector< 4, Type > &other ) noexcept : x{ other.x }, y{ other.y }, z{ other.z }
        {
        }

        inline Vector< 3, Type > &operator=( const Vector< 3, Type > &other ) noexcept
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        static constexpr Vector< 3, Type > ZeroVector();
        static constexpr Vector< 3, Type > XAxis();
        static constexpr Vector< 3, Type > YAxis();
        static constexpr Vector< 3, Type > ZAxis();
        static constexpr Vector< 3, Type > OneVector();

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
    inline Vector< 3, T > operator+( const Vector< 3, T > &first, const Vector< 3, U > &second )
    {
        return Vector< 3, T >{ first.x + static_cast< T >( second.x ),
            first.y + static_cast< T >( second.y ),
            first.z + static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator-( const Vector< 3, T > &first, const Vector< 3, U > &second )
    {
        return Vector< 3, T >{ first.x - static_cast< T >( second.x ),
            first.y - static_cast< T >( second.y ),
            first.z - static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator*( const Vector< 3, T > &first, const Vector< 3, U > &second )
    {
        return Vector< 3, T >{ first.x * static_cast< T >( second.x ),
            first.y * static_cast< T >( second.y ),
            first.z * static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator*( const Vector< 3, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x * s, vector.y * s, vector.z * s };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator*( const U scale, const Vector< 3, T > &vector )
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x * s, vector.y * s, vector.z * s };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator/( const Vector< 3, T > &first, const Vector< 3, U > &second )
    {
        return Vector< 3, T >{ first.x / static_cast< T >( second.x ),
            first.y / static_cast< T >( second.y ),
            first.z / static_cast< T >( second.z ) };
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > operator/( const Vector< 3, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        return Vector< 3, T >{ vector.x / s, vector.y / s, vector.z / s };
    }

    template < Numeric T >
    inline Vector< 3, T > &operator+=( Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        first.x += second.x;
        first.y += second.y;
        first.z += second.z;
        return first;
    }

    template < Numeric T >
    inline Vector< 3, T > &operator-=( Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        first.x -= second.x;
        first.y -= second.y;
        first.z -= second.z;
        return first;
    }

    template < Numeric T >
    inline Vector< 3, T > &operator*=( Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        first.x *= second.x;
        first.y *= second.y;
        first.z *= second.z;
        return first;
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > &operator*=( Vector< 3, T > &vector, const U scale )
    {
        const T s = static_cast< T >( scale );
        vector.x *= s;
        vector.y *= s;
        vector.z *= s;
        return vector;
    }

    template < Numeric T >
    inline Vector< 3, T > &operator/=( Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        first.x /= second.x;
        first.y /= second.y;
        first.z /= second.z;
        return first;
    }

    template < Numeric T, Numeric U >
    inline Vector< 3, T > &operator/=( Vector< 3, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );
        first.x /= s;
        first.y /= s;
        first.z /= s;
        return first;
    }

    template < Numeric T >
    inline Vector< 3, T > operator-( const Vector< 3, T > &vector )
    {
        return Vector< 3, T >{ -vector.x, -vector.y, -vector.z };
    }

    template < Numeric T >
    inline bool operator==( const Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        return AreEqual< T >( first.x, second.x ) && AreEqual< T >( first.y, second.y ) &&
               AreEqual< T >( first.z, second.z );
    }

    template < Numeric T >
    inline bool operator!=( const Vector< 3, T > &first, const Vector< 3, T > &second )
    {
        return !( first == second );
    }

    template < Numeric Type >
    inline constexpr Vector< 3, Type > Vector< 3, Type >::ZeroVector()
    {
        const Type zero = static_cast< Type >( 0 );
        return Vector< 3, Type >{ zero, zero, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 3, Type > Vector< 3, Type >::XAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, zero, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 3, Type > Vector< 3, Type >::YAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ zero, one, zero };
    }

    template < Numeric Type >
    inline constexpr Vector< 3, Type > Vector< 3, Type >::ZAxis()
    {
        const Type zero = static_cast< Type >( 0 );
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ zero, zero, one };
    }

    template < Numeric Type >
    inline constexpr Vector< 3, Type > Vector< 3, Type >::OneVector()
    {
        const Type one = static_cast< Type >( 1 );
        return Vector< 3, Type >{ one, one, one };
    }

    template < Numeric Type >
    inline float DotProduct( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template < Numeric Type >
    inline Vector< 3, Type > CrossProduct( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return Vector< 3, Type >{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    }

    template < Numeric Type >
    inline float DistanceSqr( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) +
               ( v1.z - v2.z ) * ( v1.z - v2.z );
    }

    template < Numeric Type >
    inline float Distance( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return SquareRoot(
            ( v1.x - v2.x ) * ( v1.x - v2.x ) + ( v1.y - v2.y ) * ( v1.y - v2.y ) + ( v1.z - v2.z ) * ( v1.z - v2.z ) );
    }

    template < Numeric Type >
    inline bool IsUnitVector( const Vector< 3, Type > &vector )
    {
        return IsOne( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z );
    }

    template < Numeric Type >
    inline bool HasZeroLength( const Vector< 3, Type > &vector, float precision = g_Epsilon )
    {
        return IsSquareZero( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, precision );
    }

    template < Numeric Type >
    inline bool IsZeroVector( const Vector< 3, Type > &vector, float precision = g_Epsilon )
    {
        return IsZero( vector.x, precision ) && IsZero( vector.y, precision ) && IsZero( vector.z, precision );
    }

    template < Numeric Type >
    inline Vector< 3, Type > GetPerpendicular( const Vector< 3, Type > &vector )
    {
        auto perpendicular = CrossProduct( vector, Vector< 3, Type >::XAxis() );

        if ( HasZeroLength( perpendicular ) )
        {
            perpendicular = CrossProduct( vector, Vector< 3, Type >::YAxis() );
        }

        return GetNormalized( perpendicular );
    }

    template < Numeric Type >
    inline Vector< 3, Type > GenerateRandom( const Vector< 3, Type > &minimum, const Vector< 3, Type > &maximum )
    {
        return Vector< 3, Type >{ GenerateRandom( minimum.x, maximum.x ),
            GenerateRandom( minimum.y, maximum.y ),
            GenerateRandom( minimum.z, maximum.z ) };
    }

    template < Numeric Type >
    inline Vector< 3, Type > Minimum( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return Vector< 3, Type >{ Minimum( v1.x, v2.x ), Minimum( v1.y, v2.y ), Minimum( v1.z, v2.z ) };
    }

    template < Numeric Type >
    inline Vector< 3, Type > Maximum( const Vector< 3, Type > &v1, const Vector< 3, Type > &v2 )
    {
        return Vector< 3, Type >{ Maximum( v1.x, v2.x ), Maximum( v1.y, v2.y ), Maximum( v1.z, v2.z ) };
    }
}