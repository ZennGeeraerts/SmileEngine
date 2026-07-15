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
 * @file        projection.h
 * @author      Zenn Geeraerts
 * @created		15 July 2026
 * @brief       Projection matrix functions
 */
#pragma once

#include "angle.h"
#include "matrix4.h"

namespace smile::math
{
    enum class DepthRange : Uint8
    {
        Between0And1,
        BetweenMinus1And1
    };

    void SetPerspectiveProjection( Matrix< 4, 4, Real > &matrix,
        const Real frustumLeft,
        const Real frustumRight,
        const Real frustumBottom,
        const Real frustumTop,
        const Real frustumNear,
        const Real frustumFar,
        const DepthRange depthRange = DepthRange::Between0And1 ) noexcept;

    void SetPerspectiveProjection( Matrix< 4, 4, Real > &matrix,
        const Angle fovY,
        const Real aspectRatio,
        const Real nearPlane,
        const Real farPlane,
        const DepthRange depthRange = DepthRange::Between0And1 ) noexcept;

    void SetOrthographicProjection( Matrix< 4, 4, Real > &matrix,
        const Real frustumLeft,
        const Real frustumRight,
        const Real frustumBottom,
        const Real frustumTop,
        const Real frustumNear,
        const Real frustumFar ) noexcept;
}