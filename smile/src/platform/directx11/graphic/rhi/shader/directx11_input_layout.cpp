/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_input_layout.h"

#include "directx11_shader.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "platform/directx11/graphic/rhi/dxgi_format.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    DirectX11InputLayout::~DirectX11InputLayout()
    {
        Destroy();
    }

    void DirectX11InputLayout::Create( ID3D11Device *pDevice,
        const BufferLayout &layout,
        const DirectX11Shader &vertexShader )
    {
        if ( vertexShader.Descriptor.Stage != ShaderStage::Vertex )
        {
            SM_LOG_ERROR( "DirectX11InputLayout::Create > Failed to create input layout: {}" );
            return;
        }

        std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
        for ( const auto &element : layout )
        {
            inputDescs.emplace_back( D3D11_INPUT_ELEMENT_DESC{ element.Name.c_str(),
                0,
                GetDXGIFormatMapping( element.FormatType ).SRVFormat,
                0,
                element.Offset,
                D3D11_INPUT_PER_VERTEX_DATA,
                0 } );
        }

        Uint32 count{ static_cast< Uint32 >( inputDescs.size() ) };

        HRESULT result = pDevice->CreateInputLayout(
            inputDescs.data(), count, vertexShader.ByteCode.data(), vertexShader.ByteCode.size(), &pInternal );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11InputLayout::Create > Failed to create input layout: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }
    }

    void DirectX11InputLayout::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}