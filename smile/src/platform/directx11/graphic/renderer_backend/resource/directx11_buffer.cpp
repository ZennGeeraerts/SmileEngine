/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/

#include "smpch.h"
#include "directx11_buffer.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"
#include "platform/directx11/graphic/renderer_backend/dxgi_format.h"

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

    static D3D11_BIND_FLAG BindFlagsToDirectXType( foundation::Flags< BufferBindFlags > bindFlags )
    {
        Uint32 targetFlags = 0;

        if ( bindFlags.Has( BufferBindFlags::VertexBuffer ) )
            targetFlags |= D3D11_BIND_VERTEX_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::IndexBuffer ) )
            targetFlags |= D3D11_BIND_INDEX_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::UniformBuffer ) )
            targetFlags |= D3D11_BIND_CONSTANT_BUFFER;

        if ( bindFlags.Has( BufferBindFlags::ShaderResource ) )
            targetFlags |= D3D11_BIND_SHADER_RESOURCE;

        if ( bindFlags.Has( BufferBindFlags::UnorderedAccess ) )
            targetFlags |= D3D11_BIND_UNORDERED_ACCESS;

        return static_cast< D3D11_BIND_FLAG >( targetFlags );
    }

    void DirectX11Buffer::Create( ID3D11Device *pDevice, const GPUBufferDescriptor &desc )
    {
        Descriptor = desc;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = BufferUsageToDirectXType( desc.Usage );
        bufferDesc.ByteWidth = desc.Size;
        bufferDesc.BindFlags = BindFlagsToDirectXType( desc.BindFlags );
        bufferDesc.CPUAccessFlags = BufferCPUAccessToDirectXType( desc.CPUAccess );
        bufferDesc.MiscFlags = 0;

        if ( desc.StructStride != 0 )
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

        if ( desc.AllowRawViews )
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

        bufferDesc.StructureByteStride = desc.StructStride;

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

    ID3D11ShaderResourceView *DirectX11Buffer::GetOrCreateShaderResourceView( ID3D11Device *pDevice,
        Format format,
        BufferRange bufferRange,
        ResourceType type )
    {
        SM_ASSERT(
            format != Format::UNKNOWN, "DirectX11Buffer::GetOrCreateShaderResourceView > Format cannot be unknown" );

        bufferRange = bufferRange.Resolve( Descriptor );

        BufferBindingKey bufferBindingKey{ bufferRange, format, type };
        auto srvIt = ShaderResourceViewMap.find( bufferBindingKey );
        if ( srvIt != ShaderResourceViewMap.end() )
            return srvIt->second;

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
                desc11.BufferEx.FirstElement = static_cast< UINT >( bufferRange.Offset / Descriptor.StructStride );
                desc11.BufferEx.NumElements = static_cast< UINT >( bufferRange.Size / Descriptor.StructStride );
                break;
            }
            case ResourceType::RawBuffer_SRV:
            {
                desc11.Format = DXGI_FORMAT_R32_TYPELESS;
                desc11.BufferEx.FirstElement = static_cast< UINT >( bufferRange.Offset / 4 );
                desc11.BufferEx.NumElements = static_cast< UINT >( bufferRange.Size / 4 );
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
                desc11.BufferEx.FirstElement = static_cast< UINT >( bufferRange.Offset / formatInfo.BytesPerBlock );
                desc11.BufferEx.NumElements = static_cast< UINT >( bufferRange.Size / formatInfo.BytesPerBlock );
                break;
            }
            default:
                SM_ASSERT( false, "Unsupported resource type" );
                return nullptr;
        }

        ID3D11ShaderResourceView *pShaderResourceView;
        const HRESULT result = pDevice->CreateShaderResourceView( pInternal, &desc11, &pShaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Buffer::GetOrCreateShaderResourceView > Failed to create shader resource view" );
        }

        ShaderResourceViewMap.insert( std::make_pair( bufferBindingKey, pShaderResourceView ) );

        return pShaderResourceView;
    }
}