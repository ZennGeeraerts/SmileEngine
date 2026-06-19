/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_input_layout.h"

#include "d3d11_shader.h"

#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "platform/d3dcommon/graphic/rhi/dxgi_format.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    D3D11InputLayout::~D3D11InputLayout()
    {
        Destroy();
    }

    void D3D11InputLayout::Create( ID3D11Device *pDevice, const BufferLayout &layout, const D3D11Shader &vertexShader )
    {
        if ( vertexShader.Descriptor.Stage != ShaderStage::Vertex )
        {
            SM_LOG_ERROR( "D3D11InputLayout::Create > Failed to create input layout" );
            return;
        }

        primitive::Vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
        for ( const auto &element : layout )
        {
            inputDescs.PushBack( D3D11_INPUT_ELEMENT_DESC{ element.Name.GetData(),
                element.SemanticIndex,
                GetDXGIFormatMapping( element.FormatType ).SRVFormat,
                element.IsInstanced ? 1u : 0u,
                element.Offset,
                element.IsInstanced ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA,
                element.IsInstanced ? 1u : 0u } );
        }

        UINT count{ static_cast< UINT >( inputDescs.GetItemCount() ) };

        HRESULT result = pDevice->CreateInputLayout( inputDescs.GetData(),
            count,
            vertexShader.ByteCode.GetData(),
            static_cast< SIZE_T >( vertexShader.ByteCode.GetItemCount() ),
            &pInternal );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "D3D11InputLayout::Create > Failed to create input layout: {}", GetD3DErrorMessageString( result ) );
        }
    }

    void D3D11InputLayout::Destroy()
    {
        pInternal.Reset();
    }
}