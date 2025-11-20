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
 * @file        shader_reflector.cpp
 * @author      Zenn Geeraerts
 * @created     20 November 2025
 * @brief       Shader reflection tool
 */
#include "smpch.h"
#include "shader_reflector.h"

#include "platform/directx11/graphic/tools/d3d11_shader_reflector.h"

namespace smile::graphic
{
    bool ShaderReflector::Reflect( ShaderBlobFormat blobFormat,
        const primitive::Vector< Byte > &byteCode,
        ShaderReflectionData &reflectionData )
    {
        switch ( blobFormat )
        {
            case ShaderBlobFormat::DXBC:
                return D3D11ShaderReflector::Reflect( byteCode, reflectionData );

            default:
            {
                SM_ASSERT_MSG( false,
                    "ShaderReflector::Reflect > Reflector for blob format: '{}' not implemented",
                    ShaderBlobFormatToString( blobFormat ) );
                return false;
            }
        }
    }
}