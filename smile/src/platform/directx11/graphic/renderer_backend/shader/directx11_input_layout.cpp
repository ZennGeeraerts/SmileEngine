/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_input_layout.h"

#include "directx11_shader.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

#include <d3d11.h>

namespace smile::graphic
{
    static DXGI_FORMAT ShaderDataTypeToDirectXBaseType( ShaderDataType type )
    {
        switch ( type )
        {
            case ShaderDataType::Float:
                return DXGI_FORMAT_R32_FLOAT;
            case ShaderDataType::Float2:
                return DXGI_FORMAT_R32G32_FLOAT;
            case ShaderDataType::Float3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case ShaderDataType::Float4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case ShaderDataType::Mat3:
                return DXGI_FORMAT_UNKNOWN;
            case ShaderDataType::Mat4:
                return DXGI_FORMAT_UNKNOWN;
            case ShaderDataType::Int:
                return DXGI_FORMAT_R32_SINT;
            case ShaderDataType::Int2:
                return DXGI_FORMAT_R32G32_SINT;
            case ShaderDataType::Int3:
                return DXGI_FORMAT_R32G32B32_SINT;
            case ShaderDataType::Int4:
                return DXGI_FORMAT_R32G32B32A32_SINT;
            case ShaderDataType::Bool:
                return DXGI_FORMAT_UNKNOWN;
            default:
                SM_ASSERT( false, "ShaderDataTypeToDirectXBaseType > Unknown ShaderDataType" );
                return DXGI_FORMAT_UNKNOWN;
        }
    }

    static ShaderDataType DirectXBaseTypeToShaderDataType( DXGI_FORMAT type )
    {
        switch ( type )
        {
            case DXGI_FORMAT_R32_FLOAT:
                return ShaderDataType::Float;
            case DXGI_FORMAT_R32G32_FLOAT:
                return ShaderDataType::Float2;
            case DXGI_FORMAT_R32G32B32_FLOAT:
                return ShaderDataType::Float3;
            case DXGI_FORMAT_R32G32B32A32_FLOAT:
                return ShaderDataType::Float4;
            case DXGI_FORMAT_R32_SINT:
                return ShaderDataType::Int;
            case DXGI_FORMAT_R32G32_SINT:
                return ShaderDataType::Int2;
            case DXGI_FORMAT_R32G32B32_SINT:
                return ShaderDataType::Int3;
            case DXGI_FORMAT_R32G32B32A32_SINT:
                return ShaderDataType::Int4;
            default:
                SM_ASSERT( false, "DirectXBaseTypeToShaderDataType > Unknown DXGI Type" );
                return ShaderDataType::None;
        }
    }

    DirectX11InputLayout::~DirectX11InputLayout()
    {
        Destroy();
    }

    void DirectX11InputLayout::Create( ID3D11Device *pDevice,
        const BufferLayout &layout,
        const DirectX11Shader &vertexShader )
    {
        if ( vertexShader.Descriptor.Type != ShaderType::Vertex )
        {
            SM_LOG_ERROR( "DirectX11InputLayout::Create > Failed to create input layout: {}" );
            return;
        }

        std::vector< D3D11_INPUT_ELEMENT_DESC > inputDescs{};
        for ( const auto &element : layout )
        {
            inputDescs.emplace_back( D3D11_INPUT_ELEMENT_DESC{ element.Name.c_str(),
                0,
                ShaderDataTypeToDirectXBaseType( element.DataType ),
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