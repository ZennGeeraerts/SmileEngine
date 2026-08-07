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
 * @file        quaternion.h
 * @author      Zenn Geeraerts
 * @created		16 July 2026
 * @brief       Quaternion struct and functions
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix3.h"
#include "angle.h"

namespace smile::math
{
    class Quaternion final
    {
      public:
        constexpr Quaternion() noexcept = default;

        constexpr Quaternion( const Real ix, const Real iy, const Real iz, const Real r ) noexcept
            : x{ ix }, y{ iy }, z{ iz }, w{ r }
        {
        }

        constexpr Quaternion( const Vector< 4, Real > &vector ) noexcept : Vector{ vector }
        {
        }

        constexpr Quaternion( const Quaternion & ) noexcept = default;

        ~Quaternion() noexcept = default;

        union
        {
            struct
            {
                Real x;
                Real y;
                Real z;
                Real w;
            };
            struct
            {
                Vector< 3, Real > Imaginary;
                Real r;
            };
            Vector< 4, Real > Vector;
        };
    };

    static_assert( sizeof( Quaternion ) == sizeof( Vector< 4, Real > ),
        "Quaternion and Vector< 4, Real > must be the same size" );

    constexpr void SetIdentity( Quaternion &quaternion ) noexcept
    {
        quaternion.x = 0.0f;
        quaternion.y = 0.0f;
        quaternion.z = 0.0f;
        quaternion.w = 1.0f;
    }

    Vector< 3, Real > QuaternionToEuler( const Quaternion &quaternion ) noexcept;

    constexpr void QuaternionToRotationMatrix( const Quaternion &quaternion, Matrix< 3, 3, Real > &matrix ) noexcept
    {
        const Real twoX = 2.0f * quaternion.x;
        const Real twoY = 2.0f * quaternion.y;
        const Real twoZ = 2.0f * quaternion.z;
        const Real twoWX = twoX * quaternion.w;
        const Real twoWY = twoY * quaternion.w;
        const Real twoWZ = twoZ * quaternion.w;
        const Real twoXX = twoX * quaternion.x;
        const Real twoXY = twoY * quaternion.x;
        const Real twoXZ = twoZ * quaternion.x;
        const Real twoYY = twoY * quaternion.y;
        const Real twoYZ = twoZ * quaternion.y;
        const Real twoZZ = twoZ * quaternion.z;

        matrix.xx = 1.0f - ( twoYY + twoZZ );
        matrix.xy = twoXY - twoWZ;
        matrix.xz = twoXZ + twoWY;
        matrix.yx = twoXY + twoWZ;
        matrix.yy = 1.0f - ( twoXX + twoZZ );
        matrix.yz = twoYZ - twoWX;
        matrix.zx = twoXZ - twoWY;
        matrix.zy = twoYZ + twoWX;
        matrix.zz = 1.0f - ( twoXX + twoYY );
    }

    Quaternion QuaternionFromRotationMatrix( const Matrix< 3, 3, Real > &matrix ) noexcept;
    Quaternion QuaternionFromYawPitchRoll( const Angle yaw, const Angle pitch, const Angle roll ) noexcept;
    Quaternion QuaternionFromAxisAngle( const Vector< 3, Real > &axis, const Angle angle ) noexcept;
    Quaternion Slerp( const Quaternion &start, const Quaternion &end, const Real percentage ) noexcept;
}