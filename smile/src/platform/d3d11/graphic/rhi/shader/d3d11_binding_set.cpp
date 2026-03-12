/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_binding_set.h"

#include "platform/d3d11/graphic/rhi/d3d11_device.h"
#include "smile/common/memory/memory.h"

namespace smile::graphic::rhi
{
    D3D11BindingSet::~D3D11BindingSet()
    {
        Destroy();
    }

    void
    D3D11BindingSet::Create( D3D11Device &device, const BindingSetDescriptor &descriptor, const BindingLayout &layout )
    {
        Visibility = layout.GetVisibility();

        const Uint32 constantSize = 16;

        for ( const BindingSetElement &binding : descriptor.Elements )
        {
            switch ( binding.Type )
            {
                case ResourceType::Texture_SRV:
                {
                    D3D11Texture &texture = device.m_Textures[binding.Texture.GetIndex()];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "D3D11BindingSet::Create > Shader resource view slot is not nullptr" );

                    pShaderResourceViews[binding.Slot] = texture.GetOrCreateShaderResourceView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Subresources, binding.Dimension );

                    MinSRVSlot = std::min( MinSRVSlot, binding.Slot );
                    MaxSRVSlot = std::max( MaxSRVSlot, binding.Slot );

                    break;
                }
                case ResourceType::Texture_UAV:
                {
                    D3D11Texture &texture = device.m_Textures[binding.Texture.GetIndex()];

                    SM_ASSERT( pUnorderedAccessViews[binding.Slot] == nullptr,
                        "D3D11BindingSet::Create > Unordered access view slot is not nullptr" );

                    pUnorderedAccessViews[binding.Slot] = texture.GetOrCreateUnorderedAccessView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Subresources, binding.Dimension );

                    MinUAVSlot = std::min( MinUAVSlot, binding.Slot );
                    MaxUAVSlot = std::max( MaxUAVSlot, binding.Slot );

                    break;
                }
                case ResourceType::TypedBuffer_SRV:
                case ResourceType::StructuredBuffer_SRV:
                case ResourceType::RawBuffer_SRV:
                {
                    D3D11Buffer &buffer = device.m_GPUBuffers[binding.Buffer.GetIndex()];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "D3D11BindingSet::Create > Shader resource view slot is not nullptr" );

                    pShaderResourceViews[binding.Slot] = buffer.GetOrCreateShaderResourceView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Range, binding.Type );

                    MinSRVSlot = std::min( MinSRVSlot, binding.Slot );
                    MaxSRVSlot = std::max( MaxSRVSlot, binding.Slot );

                    break;
                }
                case ResourceType::TypedBuffer_UAV:
                case ResourceType::StructuredBuffer_UAV:
                case ResourceType::RawBuffer_UAV:
                {
                    D3D11Buffer &buffer = device.m_GPUBuffers[binding.Buffer.GetIndex()];

                    SM_ASSERT( pUnorderedAccessViews[binding.Slot] == nullptr,
                        "D3D11BindingSet::Create > Unordered access view slot is not nullptr" );

                    pUnorderedAccessViews[binding.Slot] = buffer.GetOrCreateUnorderedAccessView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Range, binding.Type );

                    MinUAVSlot = std::min( MinUAVSlot, binding.Slot );
                    MaxUAVSlot = std::max( MaxUAVSlot, binding.Slot );

                    break;
                }
                case ResourceType::ConstantBuffer:
                case ResourceType::VolatileConstantBuffer:
                {
                    D3D11Buffer &buffer = device.m_GPUBuffers[binding.Buffer.GetIndex()];

                    SM_ASSERT( pConstantBuffers[binding.Slot] == nullptr,
                        "D3D11BindingSet::Create > Constant buffer slot is not nullptr" );

                    const BufferRange range = binding.Range.Resolve( buffer.Descriptor );

                    pConstantBuffers[binding.Slot] = buffer.pInternal.Get();

                    ConstantBufferOffsets[binding.Slot] = range.Offset / constantSize;
                    ConstantBufferCounts[binding.Slot] =
                        memory::Align( range.Size, s_ConstantBufferOffsetSizeAlignment ) / constantSize;

                    MinConstantBufferSlot = std::min( MinConstantBufferSlot, binding.Slot );
                    MaxConstantBufferSlot = std::max( MaxConstantBufferSlot, binding.Slot );

                    break;
                }
                case ResourceType::Sampler:
                {
                    SM_ASSERT(
                        pSamplers[binding.Slot] == nullptr, "D3D11BindingSet::Create > Sampler slot is not nullptr" );

                    const D3D11Sampler &sampler = device.m_Samplers[binding.Sampler.GetIndex()];
                    pSamplers[binding.Slot] = sampler.pInternal.Get();

                    MinSamplerSlot = std::min( MinSamplerSlot, binding.Slot );
                    MaxSamplerSlot = std::max( MaxSamplerSlot, binding.Slot );

                    break;
                }
                default:
                {
                    SM_LOG_ERROR( "D3D11BindingSet::Create > Unsupported binding" );
                    continue;
                }
            }
        }
    }

    void D3D11BindingSet::Destroy()
    {
    }

    bool D3D11BindingSet::IsSuperSetOf( const D3D11BindingSet &other ) const
    {
        return MinSRVSlot <= other.MinSRVSlot && MaxSRVSlot >= other.MaxSRVSlot && MinUAVSlot <= other.MinUAVSlot &&
               MaxUAVSlot >= other.MaxUAVSlot && MinSamplerSlot <= other.MinSamplerSlot &&
               MaxSamplerSlot >= other.MaxSamplerSlot && MinConstantBufferSlot <= other.MinConstantBufferSlot &&
               MaxConstantBufferSlot >= other.MaxConstantBufferSlot;
    }
}