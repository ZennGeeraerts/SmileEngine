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

    DirectX11Texture::~DirectX11Texture()
    {
        Destroy();
    }

    void DirectX11Texture::Create( ID3D11Device *pDevice, const std::filesystem::path &path )
    {
        DirectX::TexMetadata info{};
        if ( !LoadTexture( pDevice, path, &pInternal, &pShaderResourceView, info ) )
        {
            SAFE_RELEASE( pInternal );
            SAFE_RELEASE( pShaderResourceView );
            SM_ASSERT( false, "DirectX11Texture::Create > Failed to load texture" );
        }
    }

    void DirectX11Texture::Destroy()
    {
        SAFE_RELEASE( pInternal );
        SAFE_RELEASE( pShaderResourceView );
    }
}