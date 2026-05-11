/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material_parameter_value.h
 * @author      Zenn Geeraerts
 * @created     11 May 2026
 * @brief       Shared material parameter value type used by both asset and runtime descriptors
 */
#pragma once

#include "smile/common/primitive/collection/vector.h"

#include <DirectXMath.h>
#include <variant>

namespace smile::graphic
{
    using MaterialParameterValue = std::
        variant< bool, int, float, DirectX::XMFLOAT2, DirectX::XMFLOAT3, DirectX::XMFLOAT4, primitive::Vector< Byte > >;
}
