/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_pipeline.h"

#include "platform/directx11/graphic/renderer_backend/directx11_device.h"
#include "platform/directx11/graphic/renderer_backend/directx11_primitive_topology.h"

namespace smile::graphic
{
    void DirectX11Pipeline::Create( DirectX11Device &device, const GraphicsPipelineDescriptor &desc )
    {
        pInputLayout = device.GetOrCreateInputLayout( desc )->pInternal;
        Layout = desc.InputLayout;

        PrimitiveTopology = ConvertToDirectX11PrimitiveTopology( desc.Topology );
        pRasterizerState = device.GetOrCreateRasterizerState( desc.State.RasterizerState )->pInternal;
        pDepthStencilState = device.GetOrCreateDepthStencilState( desc.State.DepthStencilState )->pInternal;

        ShaderMask = { ShaderStage::Unknown };

        if ( desc.VertexShaderHandle.IsValid() )
        {
            pVertexShader = device.m_Shaders[desc.VertexShaderHandle.GetIndex()].pVertexShader;
            ShaderMask.Set( ShaderStage::Vertex );
        }
        if ( desc.PixelShaderHandle.IsValid() )
        {
            pPixelShader = device.m_Shaders[desc.PixelShaderHandle.GetIndex()].pPixelShader;
            ShaderMask.Set( ShaderStage::Pixel );
        }

        for ( const auto &layout : desc.BindingLayouts )
        {
            if ( !layout.GetVisibility().Has( ShaderStage::Pixel ) )
                continue;

            for ( const auto &elem : layout.GetElements() )
            {
                if ( elem.Type == ResourceType::TypedBuffer_UAV || elem.Type == ResourceType::Texture_UAV ||
                     elem.Type == ResourceType::StructuredBuffer_UAV )
                {
                    PixelShaderHasUAVs = true;
                    break;
                }
            }

            if ( PixelShaderHasUAVs )
                break;
        }
    }
}