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
 * @file        shader_compiler.cpp
 * @author      Zenn Geeraerts
 * @created     20 November 2025
 * @brief       Shader compiler tool
 */
#include "smpch.h"
#include "shader_compiler.h"

#include "platform/d3dcommon/graphic/tools/d3d_shader_compiler.h"

namespace smile::graphic
{
    bool ShaderCompiler::Compile( const primitive::StringView source,
        const primitive::String &sourceName,
        const ShaderCompileOptions &options,
        primitive::Vector< Byte > &byteCode )
    {
        switch ( options.OutputFormat )
        {
            case ShaderBlobFormat::DXBC:
            {
                D3DShaderCompiler compiler{};
                return compiler.Compile( source, sourceName, options, byteCode );
            }

            default:
            {
                SM_ASSERT_MSG( false,
                    "ShaderCompiler::Compile > Compiler for output format: '{}' not implemented",
                    ShaderBlobFormatToString( options.OutputFormat ) );
                return false;
            }
        }
    }
}