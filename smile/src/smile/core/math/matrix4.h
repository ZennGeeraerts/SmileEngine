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
 * @file        matrix4.h
 * @author      Zenn Geeraerts
 * @created		15 July 2026
 * @brief       4x4 matrix type
 */
#pragma once

#include "matrix.h"

namespace smile::math
{
    template < Numeric Type >
    class Matrix< 4, 4, Type > final
    {
      public:
        constexpr Matrix( Type _xx,
            Type _xy,
            Type _xz,
            Type _xw,
            Type _yx,
            Type _yy,
            Type _yz,
            Type _yw,
            Type _zx,
            Type _zy,
            Type _zz,
            Type _zw,
            Type _wx,
            Type _wy,
            Type _wz,
            Type _ww ) noexcept
            : xx{ _xx },
              xy{ _xy },
              xz{ _xz },
              xw{ _xw },
              yx{ _yx },
              yy{ _yy },
              yz{ _yz },
              yw{ _yw },
              zx{ _zx },
              zy{ _zy },
              zz{ _zz },
              zw{ _zw },
              wx{ _wx },
              wy{ _wy },
              wz{ _wz },
              ww{ _ww }
        {
        }

        constexpr Matrix( const Vector< 4, Type > &v1,
            const Vector< 4, Type > &v2,
            const Vector< 4, Type > &v3,
            const Vector< 4, Type > &v4 ) noexcept
            : xx{ v1.x },
              xy{ v2.x },
              xz{ v3.x },
              xw{ v4.x },
              yx{ v1.y },
              yy{ v2.y },
              yz{ v3.y },
              yw{ v4.y },
              zx{ v1.z },
              zy{ v2.z },
              zz{ v3.z },
              zw{ v4.z },
              wx{ v1.w },
              wy{ v2.w },
              wz{ v3.w },
              ww{ v4.w }
        {
        }

        static constexpr Matrix GetIdentity() noexcept
        {
            return { static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),

                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),

                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ),
                static_cast< Type >( 0 ),

                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 0 ),
                static_cast< Type >( 1 ) };
        }

        union
        {
            Type Data[4][4];
            struct
            {
                Type xx, xy, xz, xw;
                Type yx, yy, yz, yw;
                Type zx, zy, zz, zw;
                Type wx, wy, wz, ww;
            };
        };
    };

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > operator+( const Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        return { first.xx + second.xx,
            first.xy + second.xy,
            first.xz + second.xz,
            first.xw + second.xw,
            first.yx + second.yx,
            first.yy + second.yy,
            first.yz + second.yz,
            first.yw + second.yw,
            first.zx + second.zx,
            first.zy + second.zy,
            first.zz + second.zz,
            first.zw + second.zw,
            first.wx + second.wx,
            first.wy + second.wy,
            first.wz + second.wz,
            first.ww + second.ww };
    }

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > operator-( const Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        return { first.xx - second.xx,
            first.xy - second.xy,
            first.xz - second.xz,
            first.xw - second.xw,
            first.yx - second.yx,
            first.yy - second.yy,
            first.yz - second.yz,
            first.zx - second.zx,
            first.zy - second.zy,
            first.zz - second.zz,
            first.zw - second.zw,
            first.wx - second.wx,
            first.wy - second.wy,
            first.wz - second.wz,
            first.ww - second.ww };
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 4, 4, T > operator*( const Matrix< 4, 4, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx * s,
            first.xy * s,
            first.xz * s,
            first.xw * s,
            first.yx * s,
            first.yy * s,
            first.yz * s,
            first.yw * s,
            first.zx * s,
            first.zy * s,
            first.zz * s,
            first.zw * s,
            first.wx * s,
            first.wy * s,
            first.wz * s,
            first.ww * s };
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 4, 4, T > operator/( const Matrix< 4, 4, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        return { first.xx / s,
            first.xy / s,
            first.xz / s,
            first.xw / s,
            first.yx / s,
            first.yy / s,
            first.yz / s,
            first.yw / s,
            first.zx / s,
            first.zy / s,
            first.zz / s,
            first.zw / s,
            first.wx / s,
            first.wy / s,
            first.wz / s,
            first.ww / s };
    }

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > operator*( const Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        return { first.xx * second.xx + first.xy * second.yx + first.xz * second.zx + first.xw * second.wx,
            first.xx * second.xy + first.xy * second.yy + first.xz * second.zy + first.xw * second.wy,
            first.xx * second.xz + first.xy * second.yz + first.xz * second.zz + first.xw * second.wz,
            first.xx * second.xw + first.xy * second.yw + first.xz * second.zw + first.xw * second.ww,
            first.yx * second.xx + first.yy * second.yx + first.yz * second.zx + first.yw * second.wx,
            first.yx * second.xy + first.yy * second.yy + first.yz * second.zy + first.yw * second.wy,
            first.yx * second.xz + first.yy * second.yz + first.yz * second.zz + first.yw * second.wz,
            first.yx * second.xw + first.yy * second.yw + first.yz * second.zw + first.yw * second.ww,
            first.zx * second.xx + first.zy * second.yx + first.zz * second.zx + first.zw * second.wx,
            first.zx * second.xy + first.zy * second.yy + first.zz * second.zy + first.zw * second.wy,
            first.zx * second.xz + first.zy * second.yz + first.zz * second.zz + first.zw * second.wz,
            first.zx * second.xw + first.zy * second.yw + first.zz * second.zw + first.zw * second.ww,
            first.wx * second.xx + first.wy * second.yx + first.wz * second.zx + first.ww * second.wx,
            first.wx * second.xy + first.wy * second.yy + first.wz * second.zy + first.ww * second.wy,
            first.wx * second.xz + first.wy * second.yz + first.wz * second.zz + first.ww * second.wz,
            first.wx * second.xw + first.wy * second.yw + first.wz * second.zw + first.ww * second.ww };
    }

    template < Numeric Type >
    constexpr Vector< 4, Type > operator*( const Matrix< 4, 4, Type > &first, const Vector< 4, Type > &vector ) noexcept
    {
        return { first.xx * vector.x + first.yx * vector.y + first.zx * vector.z + first.wx * vector.w,
            first.xy * vector.x + first.yy * vector.y + first.zy * vector.z + first.wy * vector.w,
            first.xz * vector.x + first.yz * vector.y + first.zz * vector.z + first.wz * vector.w,
            first.xw * vector.x + first.yw * vector.y + first.zw * vector.z + first.ww * vector.w };
    }

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > &operator+=( Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        first.xx += second.xx;
        first.xy += second.xy;
        first.xz += second.xz;
        first.xw += second.xw;
        first.yx += second.yx;
        first.yy += second.yy;
        first.yz += second.yz;
        first.yw += second.yw;
        first.zx += second.zx;
        first.zy += second.zy;
        first.zz += second.zz;
        first.zw += second.zw;
        first.wx += second.wx;
        first.wy += second.wy;
        first.wz += second.wz;
        first.ww += second.ww;

        return first;
    }

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > &operator-=( Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        first.xx -= second.xx;
        first.xy -= second.xy;
        first.xz -= second.xz;
        first.xw -= second.xw;
        first.yx -= second.yx;
        first.yy -= second.yy;
        first.yz -= second.yz;
        first.yw -= second.yw;
        first.zx -= second.zx;
        first.zy -= second.zy;
        first.zz -= second.zz;
        first.zw -= second.zw;
        first.wx -= second.wx;
        first.wy -= second.wy;
        first.wz -= second.wz;
        first.ww -= second.ww;

        return first;
    }

    template < Numeric Type >
    constexpr Matrix< 4, 4, Type > &operator*=( Matrix< 4, 4, Type > &first,
        const Matrix< 4, 4, Type > &second ) noexcept
    {
        first = first * second;

        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 4, 4, T > &operator*=( Matrix< 4, 4, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx *= s;
        first.xy *= s;
        first.xz *= s;
        first.xw *= s;
        first.yx *= s;
        first.yy *= s;
        first.yz *= s;
        first.yw *= s;
        first.zx *= s;
        first.zy *= s;
        first.zz *= s;
        first.zw *= s;
        first.wx *= s;
        first.wy *= s;
        first.wz *= s;
        first.ww *= s;

        return first;
    }

    template < Numeric T, Numeric U >
    constexpr Matrix< 4, 4, T > &operator/=( Matrix< 4, 4, T > &first, const U scale ) noexcept
    {
        const T s = static_cast< T >( scale );

        first.xx /= s;
        first.xy /= s;
        first.xz /= s;
        first.xw /= s;
        first.yx /= s;
        first.yy /= s;
        first.yz /= s;
        first.yw /= s;
        first.zx /= s;
        first.zy /= s;
        first.zz /= s;
        first.zw /= s;
        first.wx /= s;
        first.wy /= s;
        first.wz /= s;
        first.ww /= s;

        return first;
    }

    template < Numeric Type >
    constexpr void SetIdentity( Matrix< 4, 4, Type > &matrix ) noexcept
    {
        matrix.xx = static_cast< Type >( 1 );
        matrix.xy = static_cast< Type >( 0 );
        matrix.xz = static_cast< Type >( 0 );
        matrix.xw = static_cast< Type >( 0 );

        matrix.yx = static_cast< Type >( 0 );
        matrix.yy = static_cast< Type >( 1 );
        matrix.yz = static_cast< Type >( 0 );
        matrix.yw = static_cast< Type >( 0 );

        matrix.zx = static_cast< Type >( 0 );
        matrix.zy = static_cast< Type >( 0 );
        matrix.zz = static_cast< Type >( 1 );
        matrix.zw = static_cast< Type >( 0 );

        matrix.wx = static_cast< Type >( 0 );
        matrix.wy = static_cast< Type >( 0 );
        matrix.wz = static_cast< Type >( 0 );
        matrix.ww = static_cast< Type >( 1 );
    }
}