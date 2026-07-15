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
 * @file        shader_reflector.h
 * @author      Zenn Geeraerts
 * @created     20 November 2025
 * @brief       Shader reflection tool
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/shader/shader_reflection_data.h"

namespace smile::graphic
{
    class ShaderReflector final
    {
      public:
        static bool Reflect( ShaderBlobFormat blobFormat,
            const primitive::Vector< Byte > &byteCode,
            ShaderReflectionData &reflectionData );
    };
}