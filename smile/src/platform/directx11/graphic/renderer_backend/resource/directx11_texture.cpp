/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "directx11_texture.h"

#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

#include <DirectXTex.h>

namespace smile::graphic
{
    static bool LoadTexture( ID3D11Device *pDevice,
        const std::filesystem::path &path,
        ID3D11Resource **ppResource,
        ID3D11ShaderResourceView **ppShaderResourceView,
        DirectX::TexMetadata &info )
    {
        auto extension = path.extension();

        DirectX::ScratchImage image{};

        HRESULT result{ S_OK };
        if ( extension == ".dds" )
        {
            result = DirectX::LoadFromDDSFile( path.c_str(), DirectX::DDS_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from DDS file failed: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return false;
            }
        }
        else if ( extension == ".tga" )
        {
            result = DirectX::LoadFromTGAFile( path.c_str(), &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from TGA file failed: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return false;
            }
        }
        else
        {
            result = DirectX::LoadFromWICFile( path.c_str(), DirectX::WIC_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Loading from WIC file failed: {}",
                    fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
                return false;
            }
        }

        result = DirectX::CreateTexture(
            pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), ppResource );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Failed to create texture: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            SAFE_RELEASE( ( *ppResource ) );
            return false;
        }

        result = DirectX::CreateShaderResourceView(
            pDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(), ppShaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Device::LoadTexture2D > Failed to create shader resource view: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
            SAFE_RELEASE( ( *ppShaderResourceView ) );
            return false;
        }

        return true;
    }

    static DXGI_FORMAT ImageFormatToDirectXType( ImageFormat format )
    {
        switch ( format )
        {
            case ImageFormat::RGB:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case ImageFormat::RGBA:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                SM_ASSERT_MSG( false, "Unsupported format" );
        }

        return DXGI_FORMAT_UNKNOWN;
    }

    void DirectX11Texture::Create( ID3D11Device *pDevice, const std::filesystem::path &path )
    {
        DirectX::TexMetadata info{};
        if ( !LoadTexture( pDevice, path, &pInternal, &pShaderResourceView, info ) )
        {
            SAFE_RELEASE( pInternal );
            SAFE_RELEASE( pShaderResourceView );
            SM_ASSERT_MSG( false, "DirectX11Texture::Create > Failed to load texture" );
        }
    }

    void DirectX11Texture::Create( ID3D11Device *pDevice, memory::Ref< const Image > pImage )
    {
        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = pImage->GetWidth();
        desc.Height = pImage->GetHeight();
        desc.MipLevels = 0;
        desc.ArraySize = 1;
        desc.Format = ImageFormatToDirectXType( pImage->GetFormat() );
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subResourceData{};
        subResourceData.pSysMem = pImage->GetData();
        subResourceData.SysMemPitch = pImage->GetDataSize();

        HRESULT result = pDevice->CreateTexture2D( &desc, &subResourceData, &pTexture2D );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Texture::Create > Failed to create texture: {}",
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );
        }
    }

    void DirectX11Texture::Destroy()
    {
        SAFE_RELEASE( pInternal );
        SAFE_RELEASE( pShaderResourceView );
    }
}