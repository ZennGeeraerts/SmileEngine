#include "smpch.h"
#include "directx11_texture.h"

#include "smile_engine/core/logger.h"
#include "smile_engine/core/application.h"

#include "platform/directx11/directx11_diagnostics.h"

namespace smile::renderer
{
    DirectX11Texture2D::DirectX11Texture2D( const std::string &file_path ) : filePath{ file_path }
    {
        directX11Context =
            static_cast< DirectX11Context * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT( directX11Context, "DirectX11Texture2D > Rendering context is not a DirectX11Context" );

        if ( !loadTexture( file_path ) )
        {
            SAFE_RELEASE( texture );
            SAFE_RELEASE( shaderResourceView );
            SM_ASSERT( false, "DirectX11Texture2D > Failed to load texture" );
        }

        auto texture_2d = static_cast< ID3D11Texture2D * >( texture );
        D3D11_TEXTURE2D_DESC tex2Ddesc;
        texture_2d->GetDesc( &tex2Ddesc );

        width = tex2Ddesc.Width;
        height = tex2Ddesc.Height;
    }

    bool DirectX11Texture2D::loadTexture( const std::string &file_path )
    {
        if ( file_path.find_last_of( '.' ) == std::string::npos )
        {
            SM_LOG_ERROR( "DirectX11Texture2D::loadTexture > Invalid file extension: %s", file_path.c_str() );
            return false;
        }

        std::string file_extension = file_path.substr( file_path.find_last_of( '.' ) + 1 );

        DirectX::TexMetadata info{};
        DirectX::ScratchImage image{};

        std::wstring file_path_wide = std::wstring{ file_path.begin(), file_path.end() };

        HRESULT result{ S_OK };
        if ( !_strcmpi( file_extension.c_str(), "dds" ) )
        {
            result = DirectX::LoadFromDDSFile( file_path_wide.c_str(), DirectX::DDS_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::loadTexture > Loading from DDS file failed: %ls",
                    getDirectX11ErrorMessage( result ) );
                return false;
            }
        }
        else if ( !_strcmpi( file_extension.c_str(), "tga" ) )
        {
            result = DirectX::LoadFromTGAFile( file_path_wide.c_str(), &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::loadTexture > Loading from TGA file failed: %ls",
                    getDirectX11ErrorMessage( result ) );
                return false;
            }
        }
        else
        {
            result = DirectX::LoadFromWICFile( file_path_wide.c_str(), DirectX::WIC_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::loadTexture > Loading from WIC file failed: %ls",
                    getDirectX11ErrorMessage( result ) );
                return false;
            }
        }

        result = DirectX::CreateTexture( directX11Context->getDevice(),
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            &texture );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Texture2D::loadTexture > Failed to create texture: %ls", getDirectX11ErrorMessage( result ) );
            SAFE_RELEASE( texture );
            return false;
        }

        result = DirectX::CreateShaderResourceView( directX11Context->getDevice(),
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            &shaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Texture2D::loadTexture > Failed to create shader resource view: %ls",
                getDirectX11ErrorMessage( result ) );
            SAFE_RELEASE( shaderResourceView );
            return false;
        }

        return true;
    }

    DirectX11Texture2D::~DirectX11Texture2D()
    {
        SAFE_RELEASE( texture );
        SAFE_RELEASE( shaderResourceView );
    }
}