/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        matrix2.h
 * @author      Zenn Geeraerts
 * @created		3 February 2026
 * @brief       2x2 matrix type
 */
#pragma once

#include "matrix.h"

namespace smile::math
{
    template < Numeric Type >
    struct Matrix< 2, 2, Type > final
    {
        union
        {
            Type Data[2][2];
            struct
            {
                Type m00, m10;
                Type m01, m11;
            };
        };

        Matrix< 2, 2, Type >() = default;

        Matrix< 2, 2, Type >( Type _00, Type _01, Type _10, Type _11 ) : m00{ _00 }, m10{ _01 }, m01{ _10 }, m11{ _11 }
        {
        }

        Matrix< 2, 2, Type >( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 )
            : m00{ v1.x }, m10{ v2.x }, m01{ v1.y }, m11{ v2.y }
        {
        }

        Matrix< 2, 2, Type >( const Matrix< 2, 2, Type > &other )
            : m00{ other.m00 }, m10{ other.m10 }, m01{ other.m01 }, m11{ other.m11 }
        {
        }

        Matrix< 2, 2, Type >( Matrix< 2, 2, Type > &&other ) noexcept
            : m00{ std::move( other.m00 ) },
              m10{ std::move( other.m10 ) },
              m01{ std::move( other.m01 ) },
              m11{ std::move( other.m11 ) }
        {
        }

        inline Matrix< 2, 2, Type > &operator=( const Matrix< 2, 2, Type > &other )
        {
            m00 = other.m00;
            m10 = other.m10;
            m01 = other.m01;
            m11 = other.m11;

            return *this;
        }
    };

    template < Numeric Type >
    inline Matrix< 2, 2, Type > operator+( const Matrix< 2, 2, Type > &first, const Matrix< 2, 2, Type > &second )
    {
        return { first.m00 + second.m00, first.m10 + second.m10, first.m01 + second.m01, first.m11 + second.m11 };
    }

    template < Numeric Type >
    inline Matrix< 2, 2, Type > operator-( const Matrix< 2, 2, Type > &first, const Matrix< 2, 2, Type > &second )
    {
        return { first.m00 - second.m00, first.m10 - second.m10, first.m01 - second.m01, first.m11 - second.m11 };
    }

    template < Numeric T, Numeric U >
    inline Matrix< 2, 2, T > operator*( const Matrix< 2, 2, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );

        return { first.m00 * s, first.m10 * s, first.m01 * s, first.m11 * s };
    }

    template < Numeric T, Numeric U >
    inline Matrix< 2, 2, T > operator/( const Matrix< 2, 2, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );

        return { first.m00 / s, first.m10 / s, first.m01 / s, first.m11 / s };
    }

    template < Numeric Type >
    inline Matrix< 2, 2, Type > operator*( const Matrix< 2, 2, Type > &lm, const Matrix< 2, 2, Type > &rm )
    {
        return { lm( 0, 0 ) * rm( 0, 0 ) + lm( 0, 1 ) * rm( 1, 0 ),
            lm( 0, 0 ) * rm( 0, 1 ) + lm( 0, 1 ) * rm( 1, 1 ),
            lm( 1, 0 ) * rm( 0, 0 ) + lm( 1, 1 ) * rm( 1, 0 ),
            lm( 1, 0 ) * rm( 0, 1 ) + lm( 1, 1 ) * rm( 1, 1 ) };
    }

    template < Numeric Type >
    inline Vector< 2, Type > operator*( const Matrix< 2, 2, Type > &first, const Vector< 2, Type > &vector )
    {
        return {
            first( 0, 0 ) * vector.x + first( 0, 1 ) * vector.y, first( 1, 0 ) * vector.x + first( 1, 1 ) * vector.y };
    }

    template< Numeric Type >
    inline Matrix< 2, 2, Type >& operator+=( Matrix< 2, 2, Type >& first, const Matrix< 2, 2, Type >& second)
    {
        first.m00 += second.m00;
        first.m01 += second.m01;
        first.m10 += second.m10;
        first.m11 += second.m11;

        return first;
    }

    template < Numeric Type >
    inline Matrix< 2, 2, Type > &operator-=( Matrix< 2, 2, Type > &first, const Matrix< 2, 2, Type > &second )
    {
        first.m00 -= second.m00;
        first.m01 -= second.m01;
        first.m10 -= second.m10;
        first.m11 -= second.m11;

        return first;
    }

    template < Numeric Type >
    inline Matrix< 2, 2, Type > &operator*=( Matrix< 2, 2, Type > &first, const Matrix< 2, 2, Type > &second )
    {
        first = first * second;

        return first;
    }

    template < Numeric T, Numeric U >
    inline Matrix< 2, 2, T > &operator*=( Matrix< 2, 2, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );

        first.m00 *= s;
        first.m01 *= s;
        first.m10 *= s;
        first.m11 *= s;

        return first;
    }

    template < Numeric T, Numeric U >
    inline Matrix< 2, 2, T > &operator/=( Matrix< 2, 2, T > &first, const U scale )
    {
        const T s = static_cast< T >( scale );

        first.m00 /= s;
        first.m01 /= s;
        first.m10 /= s;
        first.m11 /= s;

        return first;
    }
}