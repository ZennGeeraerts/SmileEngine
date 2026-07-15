/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "d3d11_texture.h"

#include "platform/d3dcommon/graphic/rhi/dxgi_format.h"
#include "platform/d3dcommon/graphic/d3d_diagnostics.h"
#include "platform/d3d11/graphic/rhi/d3d11_cpu_access.h"

using Microsoft::WRL::ComPtr;

namespace smile::graphic::rhi
{
    D3D11Texture::~D3D11Texture()
    {
        Destroy();
    }

    void D3D11Texture::Create( ID3D11Device *pDevice,
        const TextureDescriptor &desc,
        const primitive::Vector< Byte > &buffer )
    {
        Descriptor = desc;

        D3D11_USAGE usage = desc.CPUAccess == CPUAccessMode::None ? D3D11_USAGE_DEFAULT : D3D11_USAGE_STAGING;

        const DXGIFormatMapping &formatMapping = GetDXGIFormatMapping( desc.TextureFormat );
        const FormatInfo &formatInfo = GetFormatInfo( desc.TextureFormat );

        UINT bindFlags{ 0 };
        if ( desc.CPUAccess == CPUAccessMode::None )
        {
            if ( desc.BindFlags.Has( TextureBindFlags::ShaderResource ) )
                bindFlags |= D3D11_BIND_SHADER_RESOURCE;

            if ( desc.BindFlags.Has( TextureBindFlags::RenderTarget ) )
            {
                bindFlags |= ( formatInfo.HasDepth || formatInfo.HasStencil ) ? D3D11_BIND_DEPTH_STENCIL
                                                                              : D3D11_BIND_RENDER_TARGET;
            }

            if ( desc.BindFlags.Has( TextureBindFlags::UnorderedAccess ) )
                bindFlags |= D3D11_BIND_UNORDERED_ACCESS;
        }

        switch ( desc.Dimension )
        {
            case TextureDimension::Texture1D:
            case TextureDimension::Texture1DArray:
            {
                D3D11_TEXTURE1D_DESC desc11;
                desc11.Width = desc.Width;
                desc11.MipLevels = desc.MipLevelCount;
                desc11.ArraySize = desc.ArrayLength;
                desc11.Format = desc.IsTypeless ? formatMapping.ResourceFormat : formatMapping.RTVFormat;
                desc11.Usage = usage;
                desc11.BindFlags = bindFlags;
                desc11.CPUAccessFlags = CPUAccessToD3D11Type( desc.CPUAccess );
                desc11.MiscFlags = 0;

                ComPtr< ID3D11Texture1D > pTexture1D;
                const HRESULT result = [&]()
                {
                    if ( !buffer.IsEmpty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.GetData();
                        subResourceData.SysMemPitch = formatInfo.BytesPerBlock * desc11.Width;

                        return pDevice->CreateTexture1D( &desc11, &subResourceData, &pTexture1D );
                    }
                    else
                    {
                        return pDevice->CreateTexture1D( &desc11, nullptr, &pTexture1D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR(
                        "D3D11Texture::Create > Failed to create texture: {}", GetD3DErrorMessageString( result ) );
                    return;
                }

                pInternal = std::move( pTexture1D );

                break;
            }
            case TextureDimension::Texture2D:
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
            {
                D3D11_TEXTURE2D_DESC desc11;
                desc11.Width = desc.Width;
                desc11.Height = desc.Height;
                desc11.MipLevels = desc.MipLevelCount;
                desc11.ArraySize = desc.ArrayLength;
                desc11.Format = desc.IsTypeless ? formatMapping.ResourceFormat : formatMapping.RTVFormat;
                desc11.SampleDesc.Count = desc.SampleCount;
                desc11.SampleDesc.Quality = desc.SampleQuality;
                desc11.Usage = usage;
                desc11.BindFlags = bindFlags;
                desc11.CPUAccessFlags = CPUAccessToD3D11Type( desc.CPUAccess );

                if ( desc.Dimension == TextureDimension::TextureCube ||
                     desc.Dimension == TextureDimension::TextureCubeArray )
                {
                    desc11.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
                }
                else
                {
                    desc11.MiscFlags = 0;
                }

                ComPtr< ID3D11Texture2D > pTexture2D;
                const HRESULT result = [&]()
                {
                    if ( !buffer.IsEmpty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.GetData();
                        subResourceData.SysMemPitch = formatInfo.BytesPerBlock * desc11.Width;

                        return pDevice->CreateTexture2D( &desc11, &subResourceData, &pTexture2D );
                    }
                    else
                    {
                        return pDevice->CreateTexture2D( &desc11, nullptr, &pTexture2D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR(
                        "D3D11Texture::Create > Failed to create texture: {}", GetD3DErrorMessageString( result ) );
                    return;
                }

                pInternal = std::move( pTexture2D );

                break;
            }
            case TextureDimension::Texture3D:
            {
                D3D11_TEXTURE3D_DESC desc11;
                desc11.Width = desc.Width;
                desc11.Height = desc.Height;
                desc11.Depth = desc.Depth;
                desc11.MipLevels = desc.MipLevelCount;
                desc11.Format = desc.IsTypeless ? formatMapping.ResourceFormat : formatMapping.RTVFormat;
                desc11.Usage = usage;
                desc11.BindFlags = bindFlags;
                desc11.CPUAccessFlags = CPUAccessToD3D11Type( desc.CPUAccess );
                desc11.MiscFlags = 0;

                ComPtr< ID3D11Texture3D > pTexture3D;
                const HRESULT result = [&]()
                {
                    if ( !buffer.IsEmpty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.GetData();
                        subResourceData.SysMemPitch = formatInfo.BytesPerBlock * desc11.Width;

                        return pDevice->CreateTexture3D( &desc11, &subResourceData, &pTexture3D );
                    }
                    else
                    {
                        return pDevice->CreateTexture3D( &desc11, nullptr, &pTexture3D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR(
                        "D3D11Texture::Create > Failed to create texture: {}", GetD3DErrorMessageString( result ) );
                    return;
                }

                pInternal = std::move( pTexture3D );

                break;
            }
            case TextureDimension::Unknown:
            default:
                SM_ASSERT_MSG( false, "Invalid texture dimension" );
                break;
        }
    }

    void D3D11Texture::Destroy()
    {
        m_RenderTargetViewMap.Clear();
        m_DepthStencilViewMap.Clear();
        m_ShaderResourceViewMap.Clear();
        m_UnorderedAccessViewMap.Clear();
        pInternal.Reset();
    }

    ID3D11ShaderResourceView *D3D11Texture::GetOrCreateShaderResourceView( ID3D11Device *pDevice,
        Format format,
        TextureSubresourceSet subresources,
        TextureDimension dimension )
    {
        if ( format == Format::UNKNOWN )
            format = Descriptor.TextureFormat;

        if ( dimension == TextureDimension::Unknown )
            dimension = Descriptor.Dimension;

        subresources = subresources.Resolve( Descriptor, false );

        TextureBindingKey textureBindingKey{ subresources, format };
        auto srvIt = m_ShaderResourceViewMap.FindItemAtKey( textureBindingKey );
        if ( srvIt != m_ShaderResourceViewMap.end() )
            return srvIt.GetItem().Get();

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = GetDXGIFormatMapping( format ).SRVFormat;

        switch ( dimension )
        {
            case TextureDimension::Texture1D:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
                viewDesc.Texture1D.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.Texture1D.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::Texture1DArray:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
                viewDesc.Texture1DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture1DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture1DArray.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.Texture1DArray.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::Texture2D:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.Texture2D.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::Texture2DArray:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                viewDesc.Texture2DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture2DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture2DArray.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.Texture2DArray.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::TextureCube:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                viewDesc.TextureCube.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.TextureCube.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::TextureCubeArray:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                viewDesc.TextureCubeArray.First2DArrayFace = subresources.BaseArraySlice;
                viewDesc.TextureCubeArray.NumCubes = subresources.ArraySliceCount / 6;
                viewDesc.TextureCubeArray.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.TextureCubeArray.MipLevels = subresources.MipLevelCount;
                break;
            }
            case TextureDimension::Texture3D:
            {
                viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
                viewDesc.Texture3D.MostDetailedMip = subresources.BaseMipLevel;
                viewDesc.Texture3D.MipLevels = subresources.MipLevelCount;
                break;
            }
            default:
                SM_LOG_ERROR( "D3D11Texture::GetOrCreateShaderResourceView > Invalid texture dimension used" );
                return nullptr;
        }

        ComPtr< ID3D11ShaderResourceView > pShaderResourceView;
        const HRESULT result = pDevice->CreateShaderResourceView( pInternal.Get(), &viewDesc, &pShaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11Texture::GetOrCreateShaderResourceView > Failed to create shader resource view: {}",
                GetD3DErrorMessageString( result ) );
            return nullptr;
        }

        m_ShaderResourceViewMap.Insert( textureBindingKey, pShaderResourceView );

        return pShaderResourceView.Get();
    }

    ID3D11RenderTargetView *D3D11Texture::GetOrCreateRenderTargetView( ID3D11Device *pDevice,
        Format format,
        TextureSubresourceSet subresources )
    {
        if ( format == Format::UNKNOWN )
            format = Descriptor.TextureFormat;

        subresources = subresources.Resolve( Descriptor, true );

        TextureBindingKey textureBindingKey{ subresources, format };
        auto rtvIt = m_RenderTargetViewMap.FindItemAtKey( textureBindingKey );
        if ( rtvIt != m_RenderTargetViewMap.end() )
            return rtvIt.GetItem().Get();

        D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
        viewDesc.Format = GetDXGIFormatMapping( format ).RTVFormat;

        switch ( Descriptor.Dimension )
        {
            case TextureDimension::Texture1D:
                viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
                viewDesc.Texture1D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture1DArray:
                viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
                viewDesc.Texture1DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture1DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture1DArray.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2D:
                viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
                viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                viewDesc.Texture2DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture2DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture2DArray.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture3D:
                viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
                viewDesc.Texture3D.FirstWSlice = subresources.BaseArraySlice;
                viewDesc.Texture3D.WSize = subresources.ArraySliceCount;
                viewDesc.Texture3D.MipSlice = subresources.BaseMipLevel;
                break;
            default:
                SM_LOG_ERROR( "D3D11Texture::GetOrCreateRenderTargetView > Invalid texture dimension used" );
                return nullptr;
        }

        ComPtr< ID3D11RenderTargetView > pRenderTargetView;
        const HRESULT result = pDevice->CreateRenderTargetView( pInternal.Get(), &viewDesc, &pRenderTargetView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11Texture::GetOrCreateShaderResourceView > Failed to create render target view: {}",
                GetD3DErrorMessageString( result ) );
            return nullptr;
        }

        m_RenderTargetViewMap.Insert( textureBindingKey, pRenderTargetView );

        return pRenderTargetView.Get();
    }

    ID3D11DepthStencilView *D3D11Texture::GetOrCreateDepthStencilView( ID3D11Device *pDevice,
        TextureSubresourceSet subresources,
        bool isReadOnly )
    {
        subresources = subresources.Resolve( Descriptor, true );

        TextureBindingKey textureBindingKey{ subresources, Descriptor.TextureFormat, isReadOnly };
        auto dsvIt = m_DepthStencilViewMap.FindItemAtKey( textureBindingKey );
        if ( dsvIt != m_DepthStencilViewMap.end() )
            return dsvIt.GetItem().Get();

        D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc;
        viewDesc.Format = GetDXGIFormatMapping( Descriptor.TextureFormat ).RTVFormat;
        viewDesc.Flags = 0;

        if ( isReadOnly )
        {
            viewDesc.Flags |= D3D11_DSV_READ_ONLY_DEPTH;

            if ( viewDesc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT ||
                 viewDesc.Format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT )
            {
                viewDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
            }
        }

        switch ( Descriptor.Dimension )
        {
            case TextureDimension::Texture1D:
                viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
                viewDesc.Texture1D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture1DArray:
                viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
                viewDesc.Texture1DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture1DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture1DArray.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2D:
                viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
                viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                viewDesc.Texture2DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture2DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture2DArray.MipSlice = subresources.BaseMipLevel;
                break;
            default:
                SM_LOG_ERROR( "D3D11Texture::GetOrCreateDepthStencilView > Invalid texture dimension used" );
                return nullptr;
        }

        ComPtr< ID3D11DepthStencilView > pDepthStencilView;
        const HRESULT result = pDevice->CreateDepthStencilView( pInternal.Get(), &viewDesc, &pDepthStencilView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11Texture::GetOrCreateDepthStencilView > Failed to create depth stencil view: {}",
                GetD3DErrorMessageString( result ) );
            return nullptr;
        }

        m_DepthStencilViewMap.Insert( textureBindingKey, pDepthStencilView );

        return pDepthStencilView.Get();
    }

    ID3D11UnorderedAccessView *D3D11Texture::GetOrCreateUnorderedAccessView( ID3D11Device *pDevice,
        Format format,
        TextureSubresourceSet subresources,
        TextureDimension dimension )
    {
        if ( format == Format::UNKNOWN )
            format = Descriptor.TextureFormat;

        if ( dimension == TextureDimension::Unknown )
            dimension = Descriptor.Dimension;

        subresources = subresources.Resolve( Descriptor, true );

        TextureBindingKey textureBindingKey{ subresources, format };
        auto uavIt = m_UnorderedAccessViewMap.FindItemAtKey( textureBindingKey );
        if ( uavIt != m_UnorderedAccessViewMap.end() )
            return uavIt.GetItem().Get();

        D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
        viewDesc.Format = GetDXGIFormatMapping( format ).SRVFormat;

        switch ( dimension )
        {
            case TextureDimension::Texture1D:
                viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
                viewDesc.Texture1D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture1DArray:
                viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
                viewDesc.Texture1DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture1DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture1DArray.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2D:
                viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
                viewDesc.Texture2D.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
                viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
                viewDesc.Texture2DArray.FirstArraySlice = subresources.BaseArraySlice;
                viewDesc.Texture2DArray.ArraySize = subresources.ArraySliceCount;
                viewDesc.Texture2DArray.MipSlice = subresources.BaseMipLevel;
                break;
            case TextureDimension::Texture3D:
                viewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
                viewDesc.Texture3D.FirstWSlice = 0;
                viewDesc.Texture3D.WSize = Descriptor.Depth;
                viewDesc.Texture3D.MipSlice = subresources.BaseMipLevel;
                break;
            default:
                SM_LOG_ERROR( "D3D11Texture::GetOrCreateRenderTargetView > Invalid texture dimension used" );
                return nullptr;
        }

        ComPtr< ID3D11UnorderedAccessView > pUnorderedAccessView;
        const HRESULT result = pDevice->CreateUnorderedAccessView( pInternal.Get(), &viewDesc, &pUnorderedAccessView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "D3D11Texture::GetOrCreateUnorderedAccessView > Failed to create unordered access view: {}",
                GetD3DErrorMessageString( result ) );
            return nullptr;
        }

        m_UnorderedAccessViewMap.Insert( textureBindingKey, pUnorderedAccessView );

        return pUnorderedAccessView.Get();
    }
}