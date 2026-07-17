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
 * @file        quaternion.cpp
 * @author      Zenn Geeraerts
 * @created		16 July 2026
 * @brief       Quaternion struct and functions
 */
#include "smpch.h"
#include "quaternion.h"

namespace smile::math
{
    Vector< 3, Real > QuaternionToEuler( const Quaternion &quaternion ) noexcept
    {
        const Real xx = quaternion.x * quaternion.x;
        const Real yy = quaternion.y * quaternion.y;
        const Real zz = quaternion.z * quaternion.z;

        const Real m31 = 2.0f * quaternion.x * quaternion.z + 2.0f * quaternion.y * quaternion.w;
        const Real m32 = 2.0f * quaternion.y * quaternion.z - 2.0f * quaternion.x * quaternion.w;
        const Real m33 = 1.0f - 2.0f * xx - 2.0f * yy;

        const Real cy = SquareRoot( m33 * m33 + m31 * m31 );
        const Real cx = ArcTangent( -m32, cy );

        if ( cy > 16.0f * FLT_EPSILON )
        {
            const Real m12 = 2.0f * quaternion.x * quaternion.y + 2.0f * quaternion.z * quaternion.w;
            const Real m22 = 1.0f - 2.0f * xx - 2.0f * zz;

            return { cx, ArcTangent( m31, m33 ), ArcTangent( m12, m22 ) };
        }
        else
        {
            const Real m11 = 1.0f - 2.0f * yy - 2.0f * zz;
            const Real m21 = 2.0f * quaternion.x * quaternion.y - 2.0f * quaternion.z * quaternion.w;

            return { cx, 0.f, ArcTangent( -m21, m11 ) };
        }
    }

    Quaternion QuaternionFromRotationMatrix( const Matrix< 3, 3, Real > &matrix ) noexcept
    {
        SM_ASSERT_MSG(
            IsOne( AbsoluteValue( Determinant( matrix ) ) ), "Rotation matrix should not contain any scale" );

        const Real trace = matrix.xx + matrix.yy + matrix.zz;

        Vector< 4, Real > quaternionVector;
        if ( trace > 0.0f )
        {
            const Real s = SquareRoot( trace + 1.0f );
            const Real t = 0.5f * s;

            quaternionVector.x = ( matrix.zy - matrix.yz ) * t;
            quaternionVector.y = ( matrix.xz - matrix.zx ) * t;
            quaternionVector.z = ( matrix.yx - matrix.xy ) * t;
            quaternionVector.w = 0.5f * s;
        }
        else
        {
            const Index nextIndices[] = { 1, 2, 0 };

            Index i = 0;

            if ( matrix.yy > matrix.xx )
            {
                i = 1;
            }

            if ( matrix.zz > matrix.Data[i * 3][i] )
            {
                i = 2;
            }

            const Index j = nextIndices[i];
            const Index k = nextIndices[j];

            const Real s = SquareRoot( matrix.Data[i][i] - ( matrix.Data[j][j] + matrix.Data[k][k] ) + 1.0f );

            const Real t = 0.5f / s;

            quaternionVector.Data[i] = 0.5f * s;
            quaternionVector.w = ( matrix.Data[k][j] - matrix.Data[j][k] ) * t;
            quaternionVector.Data[j] = ( matrix.Data[j][i] + matrix.Data[i][j] ) * t;
            quaternionVector.Data[k] = ( matrix.Data[k][i] + matrix.Data[i][k] ) * t;
        }

        Normalize( quaternionVector );

        return { quaternionVector };
    }

    Quaternion QuaternionFromYawPitchRoll( const Angle yaw, const Angle pitch, const Angle roll ) noexcept
    {
        const Real cosYaw = Cosinus( yaw * 0.5f );
        const Real sinYaw = Sinus( yaw * 0.5f );
        const Real cosPitch = Cosinus( pitch * 0.5f );
        const Real sinPitch = Sinus( pitch * 0.5f );
        const Real cosRoll = Cosinus( roll * 0.5f );
        const Real sinRoll = Sinus( roll * 0.5f );

        Quaternion quaternion;
        quaternion.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
        quaternion.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
        quaternion.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
        quaternion.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

        return quaternion;
    }

    Quaternion QuaternionFromAxisAngle( const Vector< 3, Real > &axis, const Angle angle ) noexcept
    {
        const Real halfAngle = 0.5f * angle;
        const Real sinHalf = Sinus( halfAngle );

        Quaternion quaternion;
        quaternion.x = sinHalf * axis.x;
        quaternion.y = sinHalf * axis.y;
        quaternion.z = sinHalf * axis.z;
        quaternion.w = Cosinus( halfAngle );

        return quaternion;
    }

    Quaternion Slerp( const Quaternion &start, const Quaternion &end, const Real percentage ) noexcept
    {
        const Angle angle = Cosinus( DotProduct( start.Vector, end.Vector ) );

        if ( AbsoluteValue( angle ) > 0.0f )
        {
            const Real oneOverSinus = 1.0f / Sinus( angle );
            const Angle interpolatedAngle = angle * percentage;
            const Real coefficient0 = Sinus( angle - interpolatedAngle ) * oneOverSinus;
            const Real coefficient1 = Sinus( interpolatedAngle ) * oneOverSinus;

            Quaternion quaternion;

            quaternion.x = coefficient0 * start.x + coefficient1 * end.x;
            quaternion.y = coefficient0 * start.y + coefficient1 * end.y;
            quaternion.z = coefficient0 * start.z + coefficient1 * end.z;
            quaternion.w = coefficient0 * start.w + coefficient1 * end.w;

            Normalize( quaternion.Vector );

            return quaternion;
        }
        else
        {
            return start;
        }
    }
}