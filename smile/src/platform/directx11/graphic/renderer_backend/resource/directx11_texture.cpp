/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_texture.h"

#include "platform/directx11/graphic/renderer_backend/dxgi_format.h"
#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"
#include "platform/directx11/graphic/renderer_backend/directx11_cpu_access.h"

namespace smile::graphic
{
    void
    DirectX11Texture::Create( ID3D11Device *pDevice, const TextureDescriptor &desc, const std::vector< Byte > &buffer )
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
                desc11.CPUAccessFlags = BufferCPUAccessToD3D11Type( desc.CPUAccess );
                desc11.MiscFlags = 0;

                const HRESULT result = [&]()
                {
                    if ( !buffer.empty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.data();
                        subResourceData.SysMemPitch = static_cast< UINT >( buffer.size() );

                        return pDevice->CreateTexture1D( &desc11, &subResourceData, &pTexture1D );
                    }
                    else
                    {
                        return pDevice->CreateTexture1D( &desc11, nullptr, &pTexture1D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Texture::Create > Failed to create texture: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

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
                desc11.CPUAccessFlags = BufferCPUAccessToD3D11Type( desc.CPUAccess );

                if ( desc.Dimension == TextureDimension::TextureCube ||
                     desc.Dimension == TextureDimension::TextureCubeArray )
                {
                    desc11.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
                }
                else
                {
                    desc11.MiscFlags = 0;
                }

                const HRESULT result = [&]()
                {
                    if ( !buffer.empty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.data();
                        subResourceData.SysMemPitch = static_cast< UINT >( buffer.size() );

                        return pDevice->CreateTexture2D( &desc11, &subResourceData, &pTexture2D );
                    }
                    else
                    {
                        return pDevice->CreateTexture2D( &desc11, nullptr, &pTexture2D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Texture::Create > Failed to create texture: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

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
                desc11.CPUAccessFlags = BufferCPUAccessToD3D11Type( desc.CPUAccess );
                desc11.MiscFlags = 0;

                const HRESULT result = [&]()
                {
                    if ( !buffer.empty() )
                    {
                        D3D11_SUBRESOURCE_DATA subResourceData{};
                        subResourceData.pSysMem = buffer.data();
                        subResourceData.SysMemPitch = static_cast< UINT >( buffer.size() );

                        return pDevice->CreateTexture3D( &desc11, &subResourceData, &pTexture3D );
                    }
                    else
                    {
                        return pDevice->CreateTexture3D( &desc11, nullptr, &pTexture3D );
                    }
                }();

                if ( FAILED( result ) )
                {
                    SM_LOG_ERROR( "DirectX11Texture::Create > Failed to create texture: {}",
                        fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                    return;
                }

                break;
            }
            case TextureDimension::Unknown:
            default:
                SM_ASSERT( false, "Invalid texture dimension" );
                break;
        }
    }

    void DirectX11Texture::Destroy()
    {
        SAFE_RELEASE( pInternal );
        SAFE_RELEASE( pShaderResourceView );
    }
}