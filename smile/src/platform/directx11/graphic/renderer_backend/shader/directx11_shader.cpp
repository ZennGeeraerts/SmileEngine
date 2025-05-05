/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_shader.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

namespace smile::graphic
{
    DirectX11Shader::~DirectX11Shader()
    {
        Destroy();
    }

    void
    DirectX11Shader::Create( ID3D11Device *pDevice, const ShaderDescriptor &desc, const std::vector< Byte > &byteCode )
    {
        Descriptor = desc;
        ByteCode = byteCode;

        switch ( desc.Type )
        {
            case ShaderType::Vertex:
            {
                const HRESULT result =
                    pDevice->CreateVertexShader( byteCode.data(), byteCode.size(), nullptr, &pVertexShader );

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Shader::Create > Failed to create shader: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                }

                break;
            }
            case ShaderType::Pixel:
            {
                const HRESULT result =
                    pDevice->CreatePixelShader( byteCode.data(), byteCode.size(), nullptr, &pPixelShader );

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Shader::Create > Failed to create shader: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                }

                break;
            }
            default:
            {
                SM_LOG_ERROR( "DirectX11Shader::Create > Unsupported shader type" );
                break;
            }
        }
    }

    void DirectX11Shader::Destroy()
    {
        SAFE_RELEASE( pPtr );
    }
}