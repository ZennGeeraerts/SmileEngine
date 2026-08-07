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
 * @file        matrix3.h
 * @author      Zenn Geeraerts
 * @created		15 July 2026
 * @brief       3x3 matrix type
 */
#pragma once

#include "matrix.h"

namespace smile::math
{
    template < Numeric Type >
    class Matrix< 3, 3, Type > final
    {
      public:
        constexpr Matrix( Type _xx,
            Type _xy,
            Type _xz,
            Type _yx,
            Type _yy,
            Type _yz,
            Type _zx,
            Type _zy,
            Type _zz ) noexcept
            : xx{ _xx }, xy{ _xy }, xz{ _xz }, yx{ _yx }, yy{ _yy }, yz{ _yz }, zx{ _zx }, zy{ _zy }, zz{ _zz }
        {
        }

        constexpr Matrix( const Vector< 3, Type > &v1,
            const Vector< 3, Type > &v2,
            const Vector< 3, Type > &v3 ) noexcept
            : xx{ v1.x }, xy{ v2.x }, xz{ v3.x }, yx{ v1.y }, yy{ v2.y }, yz{ v3.y }, zx{ v1.z }, zy{ v2.z }, zz{ v3.z }
        {
        }

        static constexpr Matrix GetIdentity() noexcept
        {
            return { static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),

                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),

                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ) };
        }

        union
        {
            Type Data[3][3];
            struct
            {
                Type xx, xy, xz;
                Type yx, yy, yz;
                Type zx, zy, zz;
            };
        };
    };

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > operator+( const Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        return { first.xx + second.xx,
            first.xy + second.xy,
            first.xz + second.xz,
            first.yx + second.yx,
            first.yy + second.yy,
            first.yz + second.yz,
            first.zx + second.zx,
            first.zy + second.zy,
            first.zz + second.zz };
    }

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > operator-( const Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        return { first.xx - second.xx,
            first.xy - second.xy,
            first.xz - second.xz,
            first.yx - second.yx,
            first.yy - second.yy,
            first.yz - second.yz,
            first.zx - second.zx,
            first.zy - second.zy,
            first.zz - second.zz };
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 3, 3, T > operator*( const Matrix< 3, 3, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx * s,
            first.xy * s,
            first.xz * s,
            first.yx * s,
            first.yy * s,
            first.yz * s,
            first.zx * s,
            first.zy * s,
            first.zz * s };
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 3, 3, T > operator/( const Matrix< 3, 3, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx / s,
            first.xy / s,
            first.xz / s,
            first.yx / s,
            first.yy / s,
            first.yz / s,
            first.zx / s,
            first.zy / s,
            first.zz / s };
    }

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > operator*( const Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        return { first.xx * second.xx + first.xy * second.yx + first.xz * second.zx,
            first.xx * second.xy + first.xy * second.yy + first.xz * second.zy,
            first.xx * second.xz + first.xy * second.yz + first.xz * second.zz,
            first.yx * second.xx + first.yy * second.yx + first.yz * second.zx,
            first.yx * second.xy + first.yy * second.yy + first.yz * second.zy,
            first.yx * second.xz + first.yy * second.yz + first.yz * second.zz,
            first.zx * second.xx + first.zy * second.yx + first.zz * second.zx,
            first.zx * second.xy + first.zy * second.yy + first.zz * second.zy,
            first.zx * second.xz + first.zy * second.yz + first.zz * second.zz };
    }

    template < Numeric Type >
    constexpr Vector< 3, Type > operator*( const Matrix< 3, 3, Type > &first, const Vector< 3, Type > &vector ) noexcept
    {
        return { first.xx * vector.x + first.yx * vector.y + first.zx * vector.z,
            first.xy * vector.x + first.yy * vector.y + first.zy * vector.z,
            first.xz * vector.x + first.yz * vector.y + first.zz * vector.z };
    }

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > &operator+=( Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        first.xx += second.xx;
        first.xy += second.xy;
        first.xz += second.xz;
        first.yx += second.yx;
        first.yy += second.yy;
        first.yz += second.yz;
        first.zx += second.zx;
        first.zy += second.zy;
        first.zz += second.zz;

        return first;
    }

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > &operator-=( Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        first.xx -= second.xx;
        first.xy -= second.xy;
        first.xz -= second.xz;
        first.yx -= second.yx;
        first.yy -= second.yy;
        first.yz -= second.yz;
        first.zx -= second.zx;
        first.zy -= second.zy;
        first.zz -= second.zz;

        return first;
    }

    template < Numeric Type >
    constexpr Matrix< 3, 3, Type > &operator*=( Matrix< 3, 3, Type > &first,
        const Matrix< 3, 3, Type > &second ) noexcept
    {
        first = first * second;

        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 3, 3, T > &operator*=( Matrix< 3, 3, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx *= s;
        first.xy *= s;
        first.xz *= s;
        first.yx *= s;
        first.yy *= s;
        first.yz *= s;
        first.zx *= s;
        first.zy *= s;
        first.zz *= s;

        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 3, 3, T > &operator/=( Matrix< 3, 3, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx /= s;
        first.xy /= s;
        first.xz /= s;
        first.yx /= s;
        first.yy /= s;
        first.yz /= s;
        first.zx /= s;
        first.zy /= s;
        first.zz /= s;

        return first;
    }

    template < Numeric Type >
    constexpr void SetIdentity( Matrix< 3, 3, Type > &matrix ) noexcept
    {
        matrix.xx = static_cast< Type >( 1 );
        matrix.xy = static_cast< Type >( 0 );
        matrix.xz = static_cast< Type >( 0 );

        matrix.yx = static_cast< Type >( 0 );
        matrix.yy = static_cast< Type >( 1 );
        matrix.yz = static_cast< Type >( 0 );

        matrix.zx = static_cast< Type >( 0 );
        matrix.zy = static_cast< Type >( 0 );
        matrix.zz = static_cast< Type >( 1 );
    }

    template < Numeric Type >
    constexpr Type Determinant( const Matrix< 3, 3, Type > &matrix ) noexcept
    {
        return matrix.xx * ( matrix.yy * matrix.zz - matrix.yz * matrix.zy ) -
               matrix.xy * ( matrix.yx * matrix.zz - matrix.yz * matrix.zx ) +
               matrix.xz * ( matrix.yx * matrix.zy - matrix.yy * matrix.zx );
    }
}