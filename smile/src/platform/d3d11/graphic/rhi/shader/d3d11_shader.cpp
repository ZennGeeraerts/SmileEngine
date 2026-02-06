/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "d3d11_shader.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"

namespace smile::graphic::rhi
{
    D3D11Shader::~D3D11Shader()
    {
        Destroy();
    }

    void D3D11Shader::Create( ID3D11Device *pDevice,
        const ShaderDescriptor &desc,
        const primitive::Vector< Byte > &byteCode )
    {
        Descriptor = desc;
        ByteCode = byteCode;

        switch ( desc.Stage )
        {
            case ShaderStage::Vertex:
            {
                const HRESULT result = pDevice->CreateVertexShader(
                    byteCode.GetData(), static_cast< SIZE_T >( byteCode.GetItemCount() ), nullptr, &pVertexShader );

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR(
                        "D3D11Shader::Create > Failed to create shader: {}", fmt::ptr( GetD3DErrorMessage( result ) ) );
                }

                break;
            }
            case ShaderStage::Pixel:
            {
                const HRESULT result = pDevice->CreatePixelShader(
                    byteCode.GetData(), static_cast< SIZE_T >( byteCode.GetItemCount() ), nullptr, &pPixelShader );

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR(
                        "D3D11Shader::Create > Failed to create shader: {}", fmt::ptr( GetD3DErrorMessage( result ) ) );
                }

                break;
            }
            default:
            {
                SM_LOG_ERROR( "D3D11Shader::Create > Unsupported shader type" );
                break;
            }
        }
    }

    void D3D11Shader::Destroy()
    {
        SAFE_RELEASE( pPtr );
    }
}