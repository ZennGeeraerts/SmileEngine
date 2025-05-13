/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_binding_set.h"

#include "platform/directx11/graphic/renderer_backend/directx11_device.h"

namespace smile::graphic
{
    DirectX11BindingSet::~DirectX11BindingSet()
    {
        Destroy();
    }

    void DirectX11BindingSet::Create( const DirectX11Device &device, const BindingSetDescriptor &descriptor )
    {
        for ( const BindingSetElement &binding : descriptor.Elements )
        {
            switch ( binding.Type )
            {
                case ResourceType::Texture_SRV:
                {
                    const DirectX11Texture &texture = device.m_Textures[binding.Texture.GetIndex()];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Shader resource view slot is not nullptr" );

                    pShaderResourceViews[binding.Slot] = texture.pShaderResourceView;

                    MinSRVSlot = std::min( MinSRVSlot, binding.Slot );
                    MaxSRVSlot = std::max( MaxSRVSlot, binding.Slot );

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
                case ResourceType::TypedBuffer_SRV:
                case ResourceType::StructuredBuffer_SRV:
                case ResourceType::RawBuffer_SRV:
                {
                    const DirectX11Buffer &buffer = device.m_GPUBuffers[binding.Buffer.GetIndex()];

                    SM_ASSERT( pShaderResourceViews[binding.Slot] == nullptr,
                        "DirectX11BindingSet::Create > Shader resource view slot is not nullptr" );

                    pShaderResourceViews[binding.Slot] =
                        buffer.GetOrCreateShaderResourceView( device.m_Context.pDevice, binding.F );

                    MinSRVSlot = std::min( ret->minSRVSlot, binding.Slot );
                    MaxSRVSlot = std::max( ret->maxSRVSlot, binding.Slot );

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
}