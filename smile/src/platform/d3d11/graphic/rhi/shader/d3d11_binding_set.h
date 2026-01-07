/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/rhi/shader/binding_set.h"
#include "smile/graphic/rhi/shader/binding_layout.h"
#include "smile/common/primitive/collection/array.h"

#include <d3d11.h>

namespace smile::graphic::rhi
{
    class D3D11Device;

    struct D3D11BindingSet final
    {
        D3D11BindingSet() = default;
        ~D3D11BindingSet();

        D3D11BindingSet( const D3D11BindingSet & ) = delete;
        D3D11BindingSet( D3D11BindingSet && ) = delete;
        D3D11BindingSet &operator=( const D3D11BindingSet & ) = delete;
        D3D11BindingSet &operator=( D3D11BindingSet && ) = delete;

        void Create( D3D11Device &device, const BindingSetDescriptor &descriptor, const BindingLayout &layout );
        void Destroy();

        bool IsSuperSetOf( const D3D11BindingSet &other ) const;

        foundation::Flags< ShaderStage > Visibility{ ShaderStage::Unknown };

        primitive::Array< ID3D11ShaderResourceView *, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT >
            pShaderResourceViews{};
        Uint32 MinSRVSlot = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
        Uint32 MaxSRVSlot = 0;

        primitive::Array< ID3D11SamplerState *, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT > pSamplers{};
        Uint32 MinSamplerSlot = D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT;
        Uint32 MaxSamplerSlot = 0;

        primitive::Array< ID3D11Buffer *, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT > pConstantBuffers{};
        primitive::Array< UINT, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT > ConstantBufferOffsets;
        primitive::Array< UINT, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT > ConstantBufferCounts;
        Uint32 MinConstantBufferSlot = D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
        Uint32 MaxConstantBufferSlot = 0;

        primitive::Array< ID3D11UnorderedAccessView *, D3D11_1_UAV_SLOT_COUNT > pUnorderedAccessViews{};
        Uint32 MinUAVSlot = D3D11_1_UAV_SLOT_COUNT;
        Uint32 MaxUAVSlot = 0;
    };
}