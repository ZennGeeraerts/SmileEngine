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
    class Matrix< 2, 2, Type > final
    {
      public:
        constexpr Matrix( Type _xx, Type _xy, Type _yx, Type _yy ) noexcept : xx{ _xx }, xy{ _xy }, yx{ _yx }, yy{ _yy }
        {
        }

        constexpr Matrix( const Vector< 2, Type > &v1, const Vector< 2, Type > &v2 ) noexcept
            : xx{ v1.x }, xy{ v2.x }, yx{ v1.y }, yy{ v2.y }
        {
        }

        static constexpr Matrix GetIdentity() noexcept
        {
            return { static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ) };
        }

        union
        {
            Type Data[2][2];
            struct
            {
                Type xx, xy;
                Type yx, yy;
            };
        };
    };

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > operator+( const Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        return { first.xx + second.xx, first.xy + second.xy, first.yx + second.yx, first.yy + second.yy };
    }

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > operator-( const Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        return { first.xx - second.xx, first.xy - second.xy, first.yx - second.yx, first.yy - second.yy };
    }

    template < Numeric T, Numeric U >
    inline constexpr Matrix< 2, 2, T > operator*( const Matrix< 2, 2, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx * s, first.xy * s, first.yx * s, first.yy * s };
    }

    template < Numeric T, Numeric U >
    inline constexpr Matrix< 2, 2, T > operator/( const Matrix< 2, 2, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx / s, first.xy / s, first.yx / s, first.yy / s };
    }

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > operator*( const Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        return { first.xx * second.xx + first.xy * second.yx,
            first.xx * second.xy + first.xy * second.yy,
            first.yx * second.xx + first.yy * second.yx,
            first.yx * second.xy + first.yy * second.yy };
    }

    template < Numeric Type >
    inline constexpr Vector< 2, Type > operator*( const Matrix< 2, 2, Type > &first,
        const Vector< 2, Type > &vector ) noexcept
    {
        return { first.xx * vector.x + first.yx * vector.y, first.xy * vector.x + first.yy * vector.y };
    }

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > &operator+=( Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        first.xx += second.xx;
        first.xy += second.xy;
        first.yx += second.yx;
        first.yy += second.yy;

        return first;
    }

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > &operator-=( Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        first.xx -= second.xx;
        first.xy -= second.xy;
        first.yx -= second.yx;
        first.yy -= second.yy;

        return first;
    }

    template < Numeric Type >
    inline constexpr Matrix< 2, 2, Type > &operator*=( Matrix< 2, 2, Type > &first,
        const Matrix< 2, 2, Type > &second ) noexcept
    {
        first = first * second;

        return first;
    }

    template < Numeric T, Numeric U >
    inline constexpr Matrix< 2, 2, T > &operator*=( Matrix< 2, 2, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx *= s;
        first.xy *= s;
        first.yx *= s;
        first.yy *= s;

        return first;
    }

    template < Numeric T, Numeric U >
    inline constexpr Matrix< 2, 2, T > &operator/=( Matrix< 2, 2, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx /= s;
        first.xy /= s;
        first.yx /= s;
        first.yy /= s;

        return first;
    }

    template < Numeric Type >
    inline constexpr void SetIdentity( Matrix< 2, 2, Type > &matrix ) noexcept
    {
        matrix.xx = static_cast< Type >( 1 );
        matrix.xy = static_cast< Type >( 0 );
        matrix.yx = static_cast< Type >( 0 );
        matrix.yy = static_cast< Type >( 1 );
    }
}