/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_buffer.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

namespace smile::graphic
{
    static D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage )
    {
        switch ( bufferUsage )
        {
            case BufferUsage::Default:
                return D3D11_USAGE_DEFAULT;
            case BufferUsage::Immutable:
                return D3D11_USAGE_IMMUTABLE;
            case BufferUsage::Dynamic:
                return D3D11_USAGE_DYNAMIC;
            case BufferUsage::Staging:
                return D3D11_USAGE_STAGING;
            default:
                return D3D11_USAGE_DEFAULT;
        }
    }

    static UINT BufferCPUAccessToDirectXType( BufferCPUAccess cpuAccess )
    {
        switch ( cpuAccess )
        {
            case BufferCPUAccess::None:
                return 0;
            case BufferCPUAccess::Read:
                return D3D11_CPU_ACCESS_READ;
            case BufferCPUAccess::Write:
                return D3D11_CPU_ACCESS_WRITE;
            default:
                return 0;
        }
    }

    DirectX11Buffer::~DirectX11Buffer()
    {
        Destroy();
    }

    void DirectX11Buffer::Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc, Uint16 bindFlags )
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( desc.Usage );
        bufferDesc.ByteWidth = desc.Size;
        bufferDesc.BindFlags = bindFlags;
        bufferDesc.CPUAccessFlags = BufferCPUAccessToDirectXType( desc.CPUAccess );
        bufferDesc.MiscFlags = 0;

        HRESULT result;
        if ( desc.pData )
        {
            D3D11_SUBRESOURCE_DATA initData = { 0 };
            initData.pSysMem = desc.pData;

            result = pDevice->CreateBuffer( &bufferDesc, &initData, &pInternal );
        }
        else
        {
            result = pDevice->CreateBuffer( &bufferDesc, nullptr, &pInternal );
        }

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Buffer::Create > Failed to create vertex buffer: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }
    }

    void DirectX11Buffer::Destroy()
    {
        SAFE_RELEASE( pInternal );
    }
}