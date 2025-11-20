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
 * @file        d3d_shader_compiler.h
 * @author      Zenn Geeraerts
 * @created     19 November 2025
 * @brief       Shader compiler tool implementation for D3D
 */
#pragma once

#include "smile/graphic/tools/shader_compiler.h"

namespace smile::graphic
{
    class D3DShaderCompiler final
    {
      public:
        static bool Compile( const primitive::StringView source,
            const primitive::String &sourceName,
            const ShaderCompileOptions &options,
            primitive::Vector< Byte > &byteCode );
    };
}