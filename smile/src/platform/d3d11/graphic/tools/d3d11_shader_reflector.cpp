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
 * @file        d3d11_shader_reflector.cpp
 * @author      Zenn Geeraerts
 * @created     20 November 2025
 * @brief       Shader reflection tool implementation for D3D11
 */
#include "smpch.h"
#include "d3d11_shader_reflector.h"

#include "smile/common/foundation/numeric_cast.h"

#include <d3dcompiler.h>
#include <wrl.h>

namespace smile::graphic
{
    static rhi::Format GetFormatFromD3D11Param( const D3D11_SIGNATURE_PARAMETER_DESC &paramDesc )
    {
        switch ( paramDesc.ComponentType )
        {
            case D3D10_REGISTER_COMPONENT_FLOAT32:
                if ( paramDesc.Mask == 1 )
                    return rhi::Format::R32_FLOAT;
                else if ( paramDesc.Mask == 3 )
                    return rhi::Format::RG32_FLOAT;
                else if ( paramDesc.Mask == 7 )
                    return rhi::Format::RGB32_FLOAT;
                else if ( paramDesc.Mask == 15 )
                    return rhi::Format::RGBA32_FLOAT;
                else
                    return rhi::Format::UNKNOWN;
            case D3D10_REGISTER_COMPONENT_UINT32:
                if ( paramDesc.Mask == 1 )
                    return rhi::Format::R32_UINT;
                else if ( paramDesc.Mask == 3 )
                    return rhi::Format::RG32_UINT;
                else if ( paramDesc.Mask == 7 )
                    return rhi::Format::RGB32_UINT;
                else if ( paramDesc.Mask == 15 )
                    return rhi::Format::RGBA32_UINT;
                else
                    return rhi::Format::UNKNOWN;
            case D3D10_REGISTER_COMPONENT_SINT32:
                if ( paramDesc.Mask == 1 )
                    return rhi::Format::R32_SINT;
                else if ( paramDesc.Mask == 3 )
                    return rhi::Format::RG32_SINT;
                else if ( paramDesc.Mask == 7 )
                    return rhi::Format::RGB32_SINT;
                else if ( paramDesc.Mask == 15 )
                    return rhi::Format::RGBA32_SINT;
                else
                    return rhi::Format::UNKNOWN;
            default:
                return rhi::Format::UNKNOWN;
        }
    }

    static ConstantType GetConstantTypeFromD3D11ShaderType( const D3D11_SHADER_TYPE_DESC &typeDesc )
    {
        switch ( typeDesc.Class )
        {
            case D3D_SVC_SCALAR:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                        return ConstantType::Float;
                    case D3D_SVT_BOOL:
                        return ConstantType::Bool;
                    case D3D_SVT_INT:
                        return ConstantType::Int;
                    default:
                        return ConstantType::Unknown;
                }
            }
            case D3D_SVC_VECTOR:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        switch ( typeDesc.Columns )
                        {
                            case 2:
                                return ConstantType::Float2;
                            case 3:
                                return ConstantType::Float3;
                            default:
                                return ConstantType::Float3;
                        }
                    }
                    default:
                        return ConstantType::Unknown;
                }
            }
            case D3D_SVC_MATRIX_COLUMNS:
            {
                switch ( typeDesc.Type )
                {
                    case D3D_SVT_FLOAT:
                    {
                        if ( ( typeDesc.Columns == 4 ) && ( typeDesc.Rows == 4 ) )
                        {
                            if ( typeDesc.Elements == 0 )
                                return ConstantType::Mat4;
                            else
                                return ConstantType::Mat4Array;
                        }

                        return ConstantType::Unknown;
                    }
                    default:
                        return ConstantType::Unknown;
                }
            }
            default:
                return ConstantType::Unknown;
        }
    }

    static rhi::ResourceType GetResourceTypeFromD3D11ShaderInput( const D3D11_SHADER_INPUT_BIND_DESC &resDesc )
    {
        switch ( resDesc.Type )
        {
            case D3D_SIT_CBUFFER:
                return rhi::ResourceType::ConstantBuffer;

            case D3D_SIT_TBUFFER:
                return rhi::ResourceType::TypedBuffer_SRV;

            case D3D_SIT_TEXTURE:
                return rhi::ResourceType::Texture_SRV;

            case D3D_SIT_SAMPLER:
                return rhi::ResourceType::Sampler;

            case D3D_SIT_UAV_RWTYPED:
                switch ( resDesc.Dimension )
                {
                    case D3D_SRV_DIMENSION_BUFFER:
                    case D3D_SRV_DIMENSION_BUFFEREX:
                        return rhi::ResourceType::TypedBuffer_UAV;
                    case D3D_SRV_DIMENSION_TEXTURE1D:
                    case D3D_SRV_DIMENSION_TEXTURE1DARRAY:
                    case D3D_SRV_DIMENSION_TEXTURE2D:
                    case D3D_SRV_DIMENSION_TEXTURE2DARRAY:
                    case D3D_SRV_DIMENSION_TEXTURE3D:
                        return rhi::ResourceType::Texture_UAV;
                    default:
                        return rhi::ResourceType::Unknown;
                }

            case D3D_SIT_UAV_RWSTRUCTURED:
            case D3D_SIT_UAV_APPEND_STRUCTURED:
            case D3D_SIT_UAV_CONSUME_STRUCTURED:
                if ( resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER || resDesc.Dimension == D3D_SRV_DIMENSION_BUFFEREX )
                    return rhi::ResourceType::StructuredBuffer_UAV;
                else
                    return rhi::ResourceType::Unknown;

            case D3D_SIT_UAV_RWBYTEADDRESS:
                if ( resDesc.Dimension == D3D_SRV_DIMENSION_BUFFER || resDesc.Dimension == D3D_SRV_DIMENSION_BUFFEREX )
                    return rhi::ResourceType::RawBuffer_UAV;
                else
                    return rhi::ResourceType::Unknown;

            case D3D_SIT_STRUCTURED:
                return rhi::ResourceType::StructuredBuffer_SRV;

            case D3D_SIT_BYTEADDRESS:
                return rhi::ResourceType::RawBuffer_SRV;

            case D3D_SIT_RTACCELERATIONSTRUCTURE:
                return rhi::ResourceType::RayTracingAccelStruct;

            default:
                return rhi::ResourceType::Unknown;
        }
    }

    bool D3D11ShaderReflector::Reflect( const primitive::Vector< Byte > &byteCode,
        ShaderReflectionData &reflectionData )
    {
        Microsoft::WRL::ComPtr< ID3D11ShaderReflection > pReflector;
        if ( FAILED( D3DReflect( byteCode.GetData(),
                 foundation::NumericCast< SIZE_T >( byteCode.GetItemCount() ),
                 IID_PPV_ARGS( &pReflector ) ) ) )
        {
            SM_LOG_ERROR( "Failed to reflect shader" );
            return false;
        }

        D3D11_SHADER_DESC shaderDesc;
        pReflector->GetDesc( &shaderDesc );

        for ( UINT i{ 0 }; i < shaderDesc.InputParameters; ++i )
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            if ( FAILED( pReflector->GetInputParameterDesc( i, &paramDesc ) ) )
            {
                SM_LOG_ERROR( "Failed to reflect input parameters" );
                return false;
            }

            rhi::BufferElement element;
            element.Name = paramDesc.SemanticName;
            element.FormatType = GetFormatFromD3D11Param( paramDesc );

            reflectionData.InputSignature.AddElement( element );
        }

        for ( UINT i{ 0 }; i < shaderDesc.OutputParameters; ++i )
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            if ( FAILED( pReflector->GetOutputParameterDesc( i, &paramDesc ) ) )
            {
                SM_LOG_ERROR( "Failed to reflect output parameters" );
                return false;
            }

            rhi::BufferElement element;
            element.Name = paramDesc.SemanticName;
            element.FormatType = GetFormatFromD3D11Param( paramDesc );

            reflectionData.OutputSignature.AddElement( element );
        }

        for ( UINT i{ 0 }; i < shaderDesc.ConstantBuffers; ++i )
        {
            auto cb = pReflector->GetConstantBufferByIndex( i );
            D3D11_SHADER_BUFFER_DESC d3d11CBDesc;
            if ( SUCCEEDED( cb->GetDesc( &d3d11CBDesc ) ) )
            {
                ConstantBufferDescriptor cbDesc;

                for ( UINT j{ 0 }; j < d3d11CBDesc.Variables; ++j )
                {
                    auto pVar = cb->GetVariableByIndex( j );

                    D3D11_SHADER_VARIABLE_DESC varDesc;
                    if ( SUCCEEDED( pVar->GetDesc( &varDesc ) ) )
                    {
                        ID3D11ShaderReflectionType *pType = pVar->GetType();
                        D3D11_SHADER_TYPE_DESC typeDesc;
                        pType->GetDesc( &typeDesc );

                        ConstantBufferItem cbItem;
                        cbItem.Name = varDesc.Name;
                        cbItem.Type = GetConstantTypeFromD3D11ShaderType( typeDesc );
                        cbItem.Size = varDesc.Size;
                        cbItem.Offset = varDesc.StartOffset;

                        cbDesc.Add( cbItem );
                    }
                }

                reflectionData.ConstantBufferDescs.Insert( d3d11CBDesc.Name, std::move( cbDesc ) );
            }
        }

        for ( UINT i{ 0 }; i < shaderDesc.BoundResources; ++i )
        {
            D3D11_SHADER_INPUT_BIND_DESC resDesc;
            pReflector->GetResourceBindingDesc( i, &resDesc );

            rhi::BindingLayoutElement bindingLayoutElement{
                resDesc.BindPoint, GetResourceTypeFromD3D11ShaderInput( resDesc ) };

            reflectionData.ShaderResourceBindings.Insert( resDesc.Name, std::move( bindingLayoutElement ) );
        }

        return true;
    }
}