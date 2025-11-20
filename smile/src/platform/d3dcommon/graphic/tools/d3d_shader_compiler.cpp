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
 * @file        d3d_shader_compiler.cpp
 * @author      Zenn Geeraerts
 * @created     19 November 2025
 * @brief       Shader compiler tool implementation for D3D
 */
#include "smpch.h"
#include "d3d_shader_compiler.h"

#include "smile/common/foundation/numeric_cast.h"

#include <d3dcompiler.h>
#include <wrl.h>

namespace smile::graphic
{
    bool D3DShaderCompiler::CompileImpl( const primitive::StringView source,
        const primitive::String &sourceName,
        const ShaderCompileOptions &options,
        primitive::Vector< Byte > &byteCode ) const
    {
        Microsoft::WRL::ComPtr< ID3DBlob > pCompiledBlob;
        Microsoft::WRL::ComPtr< ID3DBlob > pErrorBlob;

        UINT flags{ 0 };

        if ( options.Debug )
            flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
        else if ( options.Optimize )
            flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;

        const HRESULT hr = D3DCompile( source.GetData(),
            foundation::NumericCast< SIZE_T >( source.GetCharCount() ),
            sourceName.GetData(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            options.EntryPoint.GetData(),
            options.TargetProfile.GetData(),
            flags,
            0,
            &pCompiledBlob,
            &pErrorBlob );

        if ( FAILED( hr ) )
        {
            if ( pErrorBlob )
            {
                SM_LOG_ERROR(
                    "Shader compile error: {}", reinterpret_cast< char * >( pErrorBlob->GetBufferPointer() ) );
            }
            else
            {
                SM_LOG_ERROR( "Unknown shader compile error." );
            }

            return false;
        }

        byteCode = { reinterpret_cast< Byte * >( pCompiledBlob->GetBufferPointer() ),
            foundation::NumericCast< Count >( pCompiledBlob->GetBufferSize() ) };

        return true;
    }
}