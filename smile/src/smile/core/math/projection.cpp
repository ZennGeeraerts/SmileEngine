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
 * @file        projection.cpp
 * @author      Zenn Geeraerts
 * @created		15 July 2026
 * @brief       Projection matrix functions
 */
#include "smpch.h"
#include "projection.h"

#include "math_utilities.h"

namespace smile::math
{
    void SetPerspectiveProjection( Matrix< 4, 4, Real > &matrix,
        const Real frustumLeft,
        const Real frustumRight,
        const Real frustumBottom,
        const Real frustumTop,
        const Real frustumNear,
        const Real frustumFar,
        const DepthRange depthRange ) noexcept
    {
        SM_ASSERT( frustumLeft < frustumRight );
        SM_ASSERT( frustumBottom < frustumTop );
        SM_ASSERT( 0 < frustumNear && frustumNear < frustumFar );

        SetIdentity( matrix );

        const Real oneOverRightMinusLeft = 1.0f / ( frustumRight - frustumLeft );
        const Real oneOverTopMinusBottom = 1.0f / ( frustumTop - frustumBottom );
        const Real oneOverFarMinusNear = 1.0f / ( frustumFar - frustumNear );
        const Real front2 = 2.0f * frustumNear;

        if ( depthRange == DepthRange::Between0And1 )
        {
            matrix.xx = front2 * oneOverRightMinusLeft;
            matrix.yy = front2 * oneOverTopMinusBottom;
            matrix.zx = -( frustumRight + frustumLeft ) * oneOverRightMinusLeft;
            matrix.zy = -( frustumTop + frustumBottom ) * oneOverTopMinusBottom;
            matrix.zz = frustumFar * oneOverFarMinusNear;
            matrix.zw = 1.0f;
            matrix.wz = -frustumNear * frustumFar * oneOverFarMinusNear;
            matrix.ww = 0.0f;
        }
        else
        {
            matrix.xx = front2 * oneOverRightMinusLeft;
            matrix.yy = front2 * oneOverTopMinusBottom;
            matrix.zx = -( frustumRight + frustumLeft ) * oneOverRightMinusLeft;
            matrix.zy = -( frustumTop + frustumBottom ) * oneOverTopMinusBottom;
            matrix.zz = ( frustumFar + frustumNear ) * oneOverFarMinusNear;
            matrix.zw = 1.0f;
            matrix.wz = -front2 * frustumFar * oneOverFarMinusNear;
            matrix.ww = 0.0f;
        }
    }

    void SetPerspectiveProjection( Matrix< 4, 4, Real > &matrix,
        const Angle fovY,
        const Real aspectRatio,
        const Real nearPlane,
        const Real farPlane,
        const DepthRange depthRange ) noexcept
    {
        const Real yMax = nearPlane * Tangent( fovY * 0.5f );
        const Real xMax = yMax * aspectRatio;

        SetPerspectiveProjection( matrix, -xMax, xMax, -yMax, yMax, nearPlane, farPlane, depthRange );
    }

    void SetOrthographicProjection( Matrix< 4, 4, Real > &matrix,
        const Real frustumLeft,
        const Real frustumRight,
        const Real frustumBottom,
        const Real frustumTop,
        const Real frustumNear,
        const Real frustumFar ) noexcept
    {
        SM_ASSERT( frustumLeft < frustumRight );
        SM_ASSERT( frustumNear < frustumFar );

        SetIdentity( matrix );

        const Real rightLeft = 1.0f / ( frustumRight - frustumLeft );
        const Real topBottom = 1.0f / ( frustumTop - frustumBottom );
        const Real farNear = 1.0f / ( frustumFar - frustumNear );

        matrix.xx = 2.0f * rightLeft;
        matrix.yy = 2.0f * topBottom;
        matrix.zz = -1.0f * farNear;
        matrix.wx = -( frustumRight + frustumLeft ) * rightLeft;
        matrix.wy = -( frustumTop + frustumBottom ) * topBottom;
        matrix.wz = -frustumNear * farNear;
        matrix.ww = 1.0f;
    }
}