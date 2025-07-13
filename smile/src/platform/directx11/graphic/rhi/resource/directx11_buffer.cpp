/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_buffer.h"

#include "platform/directx11/graphic/rhi/directx11_diagnostics.h"
#include "platform/directx11/graphic/rhi/dxgi_format.h"
#include "platform/directx11/graphic/rhi/directx11_cpu_access.h"

using Microsoft::WRL::ComPtr;

namespace smile::graphic::rhi
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

    static D3D11_BIND_FLAG BindFlagsToDirectXType( foundation::Flags< BufferBindFlags > bindFlags )
    {
        Uint32 targetFlags = 0;

        if ( bindFlags.Has( BufferBindFlags::VertexBuffer ) )
            targetFlags |= D3D11_BIND_VERTEX_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::IndexBuffer ) )
            targetFlags |= D3D11_BIND_INDEX_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::ConstantBuffer ) )
            targetFlags |= D3D11_BIND_CONSTANT_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::ShaderResource ) )
            targetFlags |= D3D11_BIND_SHADER_RESOURCE;

        if ( bindFlags.Has( BufferBindFlags::UnorderedAccess ) )
            targetFlags |= D3D11_BIND_UNORDERED_ACCESS;

        return static_cast< D3D11_BIND_FLAG >( targetFlags );
    }

    void DirectX11Buffer::Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc, void *pData )
    {
        Descriptor = desc;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( desc.Usage );
        bufferDesc.ByteWidth = desc.Size;
        bufferDesc.BindFlags = BindFlagsToDirectXType( desc.BindFlags );
        bufferDesc.CPUAccessFlags = CPUAccessToD3D11Type( desc.CPUAccess );
        bufferDesc.MiscFlags = 0;

        if ( desc.StructStride != 0 )
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        if ( desc.AllowRawViews )
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

        bufferDesc.StructureByteStride = desc.StructStride;

        const HRESULT result = [&]()
        {
            if ( pData )
            {
                D3D11_SUBRESOURCE_DATA initData{ 0 };
                initData.pSysMem = pData;

                return pDevice->CreateBuffer( &bufferDesc, &initData, &pInternal );
            }
            else
            {
                return pDevice->CreateBuffer( &bufferDesc, nullptr, &pInternal );
            }
        }();

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

    ID3D11ShaderResourceView *DirectX11Buffer::GetOrCreateShaderResourceView( ID3D11Device *pDevice,
        Format format,
        BufferRange bufferRange,
        ResourceType type )
    {
        if ( format == Format::UNKNOWN )
            format = Descriptor.BufferFormat;

        bufferRange = bufferRange.Resolve( Descriptor );

        BufferBindingKey bufferBindingKey{ bufferRange, format, type };
        auto srvIt = m_ShaderResourceViewMap.find( bufferBindingKey );
        if ( srvIt != m_ShaderResourceViewMap.end() )
            return srvIt->second.Get();

        D3D11_SHADER_RESOURCE_VIEW_DESC desc11;
        desc11.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
        desc11.BufferEx.Flags = 0;

        switch ( type )
        {
            case ResourceType::StructuredBuffer_SRV:
            {
                SM_ASSERT( Descriptor.StructStride != 0,
                    "DirectX11Buffer::GetOrCreateShaderResourceView > Struct stride must be known for structured "
                    "buffers" );

                desc11.Format = DXGI_FORMAT_UNKNOWN;
                desc11.BufferEx.FirstElement = bufferRange.Offset / Descriptor.StructStride;
                desc11.BufferEx.NumElements = bufferRange.Size / Descriptor.StructStride;
                break;
            }
            case ResourceType::RawBuffer_SRV:
            {
                desc11.Format = DXGI_FORMAT_R32_TYPELESS;
                desc11.BufferEx.FirstElement = bufferRange.Offset / 4;
                desc11.BufferEx.NumElements = bufferRange.Size / 4;
                desc11.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
                break;
            }
            case ResourceType::TypedBuffer_SRV:
            {
                SM_ASSERT( format != Format::UNKNOWN,
                    "DirectX11Buffer::GetOrCreateShaderResourceView > Format must be known for typed buffer" );

                const DXGIFormatMapping &formatMapping = GetDXGIFormatMapping( format );
                const FormatInfo &formatInfo = GetFormatInfo( format );

                desc11.Format = formatMapping.SRVFormat;
                desc11.BufferEx.FirstElement = bufferRange.Offset / formatInfo.BytesPerBlock;
                desc11.BufferEx.NumElements = bufferRange.Size / formatInfo.BytesPerBlock;
                break;
            }
            default:
                SM_ASSERT( false, "Unsupported resource type" );
                return nullptr;
        }

        ComPtr< ID3D11ShaderResourceView > pShaderResourceView;
        const HRESULT result = pDevice->CreateShaderResourceView( pInternal, &desc11, &pShaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Buffer::GetOrCreateShaderResourceView > Failed to create shader resource view" );
            return nullptr;
        }

        m_ShaderResourceViewMap.insert( std::make_pair( bufferBindingKey, pShaderResourceView ) );

        return pShaderResourceView.Get();
    }

    ID3D11UnorderedAccessView *DirectX11Buffer::GetOrCreateUnorderedAccessView( ID3D11Device *pDevice,
        Format format,
        BufferRange bufferRange,
        ResourceType type )
    {
        if ( format == Format::UNKNOWN )
            format = Descriptor.BufferFormat;

        bufferRange = bufferRange.Resolve( Descriptor );

        BufferBindingKey bufferBindingKey{ bufferRange, format, type };
        auto uavIt = m_UnorderedAccessViewMap.find( bufferBindingKey );
        if ( uavIt != m_UnorderedAccessViewMap.end() )
            return uavIt->second.Get();

        D3D11_UNORDERED_ACCESS_VIEW_DESC desc11;
        desc11.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        desc11.Buffer.Flags = 0;

        switch ( type )
        {
            case ResourceType::StructuredBuffer_UAV:
            {
                SM_ASSERT( Descriptor.StructStride != 0,
                    "DirectX11Buffer::GetOrCreateUnorderedAccessView > Struct stride must be known for structured "
                    "buffers" );

                desc11.Format = DXGI_FORMAT_UNKNOWN;
                desc11.Buffer.FirstElement = bufferRange.Offset / Descriptor.StructStride;
                desc11.Buffer.NumElements = bufferRange.Size / Descriptor.StructStride;
                break;
            }
            case ResourceType::RawBuffer_UAV:
            {
                desc11.Format = DXGI_FORMAT_R32_TYPELESS;
                desc11.Buffer.FirstElement = bufferRange.Offset / 4;
                desc11.Buffer.NumElements = bufferRange.Size / 4;
                desc11.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
                break;
            }
            case ResourceType::TypedBuffer_UAV:
            {
                SM_ASSERT( format != Format::UNKNOWN,
                    "DirectX11Buffer::GetOrCreateUnorderedAccessView > Format must be known for typed buffer" );

                const DXGIFormatMapping &formatMapping = GetDXGIFormatMapping( format );
                const FormatInfo &formatInfo = GetFormatInfo( format );

                desc11.Format = formatMapping.SRVFormat;
                desc11.Buffer.FirstElement = bufferRange.Offset / formatInfo.BytesPerBlock;
                desc11.Buffer.NumElements = bufferRange.Size / formatInfo.BytesPerBlock;

                break;
            }
            default:
                SM_ASSERT( false, "Unsupported resource type" );
                return nullptr;
        }

        ComPtr< ID3D11UnorderedAccessView > pUnorderedAccessView;
        const HRESULT result = pDevice->CreateUnorderedAccessView( pInternal, &desc11, &pUnorderedAccessView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Buffer::GetOrCreateUnorderedAccessView > Failed to create unordered access view" );
            return nullptr;
        }

        m_UnorderedAccessViewMap.insert( std::make_pair( bufferBindingKey, pUnorderedAccessView ) );

        return pUnorderedAccessView.Get();
    }
}