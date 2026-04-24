/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_pipeline.h"

#include "platform/d3d11/graphic/rhi/d3d11_device.h"
#include "platform/d3d11/graphic/rhi/d3d11_primitive_topology.h"

namespace smile::graphic::rhi
{
    void D3D11Pipeline::Create( D3D11Device &device, const GraphicsPipelineDescriptor &desc )
    {
        pInputLayout = device.GetOrCreateInputLayout( desc )->pInternal.Get();
        Layout = desc.InputLayout;

        PrimitiveTopology = ConvertToD3D11PrimitiveTopology( desc.Topology );
        pRasterizerState = device.GetOrCreateRasterizerState( desc.State.RasterizerState )->pInternal.Get();
        pDepthStencilState = device.GetOrCreateDepthStencilState( desc.State.DepthStencilState )->pInternal.Get();

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

        for ( const auto &layoutHandle : desc.BindingLayouts )
        {
            SM_ASSERT( device.IsHandleValid( layoutHandle, device.m_BindingLayouts ) );

            const auto &layout = device.m_BindingLayouts[layoutHandle.GetIndex()];

            if ( !layout.Internal.GetVisibility().Has( ShaderStage::Pixel ) )
                continue;

            for ( const auto &elem : layout.Internal.GetElements() )
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