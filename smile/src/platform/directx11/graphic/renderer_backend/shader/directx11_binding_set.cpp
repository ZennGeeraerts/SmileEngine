/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_binding_set.h"

#include "platform/directx11/graphic/renderer_backend/directx11_device.h"
#include "smile/common/memory/memory.h"

namespace smile::graphic
{
    DirectX11BindingSet::~DirectX11BindingSet()
    {
        Destroy();
    }

    void DirectX11BindingSet::Create( DirectX11Device &device,
        const BindingSetDescriptor &descriptor,
        const BindingLayout &layout )
    {
        Visibility = layout.GetVisibility();

        const Uint32 constantSize = 16;

        for ( const BindingSetElement &binding : descriptor.Elements )
        {
            switch ( binding.Type )
            {
                case ResourceType::Texture_SRV:
                {
                    DirectX11Texture &texture = device.m_Textures[binding.Texture.GetIndex()];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Shader resource view slot is not nullptr" );

                    pShaderResourceViews[binding.Slot] = texture.GetOrCreateShaderResourceView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Subresources, binding.Dimension );

                    MinSRVSlot = std::min( MinSRVSlot, binding.Slot );
                    MaxSRVSlot = std::max( MaxSRVSlot, binding.Slot );

                    break;
                }
                case ResourceType::Texture_UAV:
                {
                    DirectX11Texture &texture = device.m_Textures[binding.Texture.GetIndex()];

                    SM_ASSERT( pUnorderedAccessViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Unordered access view slot is not nullptr" );

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
                    DirectX11Buffer &buffer = device.m_GPUBuffers[binding.Slot];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Shader resource view slot is not nullptr" );

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
                    DirectX11Buffer &buffer = device.m_GPUBuffers[binding.Slot];

                    SM_ASSERT( pUnorderedAccessViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Unordered access view slot is not nullptr" );

                    pUnorderedAccessViews[binding.Slot] = buffer.GetOrCreateUnorderedAccessView(
                        device.m_Context.pDevice, binding.BindingFormat, binding.Range, binding.Type );

                    MinUAVSlot = std::min( MinUAVSlot, binding.Slot );
                    MaxUAVSlot = std::max( MaxUAVSlot, binding.Slot );

                    break;
                }
                case ResourceType::ConstantBuffer:
                case ResourceType::VolatileConstantBuffer:
                {
                    DirectX11Buffer &buffer = device.m_GPUBuffers[binding.Slot];

                    SM_ASSERT( pConstantBuffers[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Constant buffer slot is not nullptr" );

                    const BufferRange range = binding.Range.Resolve( buffer.Descriptor );

                    pConstantBuffers[binding.Slot] = buffer.pInternal;

                    ConstantBufferOffsets[binding.Slot] = range.Offset / constantSize;
                    ConstantBufferCounts[binding.Slot] =
                        memory::Align( range.Size, s_ConstantBufferOffsetSizeAlignment ) / constantSize;

                    MinConstantBufferSlot = std::min( MinConstantBufferSlot, binding.Slot );
                    MaxConstantBufferSlot = std::max( MaxConstantBufferSlot, binding.Slot );

                    break;
                }
                case ResourceType::Sampler:
                {
                    SM_ASSERT( pSamplers[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Sampler slot is not nullptr" );

                    const DirectX11Sampler &sampler = device.m_Samplers[binding.Sampler.GetIndex()];
                    pSamplers[binding.Slot] = sampler.pInternal;

                    MinSamplerSlot = std::min( MinSamplerSlot, binding.Slot );
                    MaxSamplerSlot = std::max( MaxSamplerSlot, binding.Slot );

                    break;
                }
                default:
                {
                    SM_LOG_ERROR( "DirectX11BindingSet::Create > Unsupported binding" );
                    continue;
                }
            }
        }
    }

    void DirectX11BindingSet::Destroy()
    {
    }

    bool DirectX11BindingSet::IsSuperSetOf( const DirectX11BindingSet &other ) const
    {
        return MinSRVSlot <= other.MinSRVSlot && MaxSRVSlot >= other.MaxSRVSlot && MinUAVSlot <= other.MinUAVSlot &&
               MaxUAVSlot >= other.MaxUAVSlot && MinSamplerSlot <= other.MinSamplerSlot &&
               MaxSamplerSlot >= other.MaxSamplerSlot && MinConstantBufferSlot <= other.MinConstantBufferSlot &&
               MaxConstantBufferSlot >= other.MaxConstantBufferSlot;
    }
}