#include "smpch.h"
#include "DirectX11Texture.h"

#include "SmileEngine/Core/Logger.h"
#include "SmileEngine/Core/Application.h"

#include "DirectX11Diagnostics.h"

namespace smile
{
    DirectX11Texture2D::DirectX11Texture2D( const std::string &filePath ) : m_FilePath{ filePath }
    {
        m_pDirectX11Context =
            static_cast< DirectX11Context * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT( m_pDirectX11Context, "DirectX11Texture2D > Rendering context is not a DirectX11Context" );

        if ( !LoadTexture( filePath ) )
        {
            SAFE_RELEASE( m_pTexture );
            SAFE_RELEASE( m_pShaderResourceView );
            SM_ASSERT( false, "DirectX11Texture2D > Failed to load texture" );
        }

        auto pTex2D = static_cast< ID3D11Texture2D * >( m_pTexture );
        D3D11_TEXTURE2D_DESC tex2Ddesc;
        pTex2D->GetDesc( &tex2Ddesc );

        m_Width = tex2Ddesc.Width;
        m_Height = tex2Ddesc.Height;
    }

    bool DirectX11Texture2D::LoadTexture( const std::string &filePath )
    {
        if ( filePath.find_last_of( '.' ) == std::string::npos )
        {
            SM_LOG_ERROR( "DirectX11Texture2D::LoadTexture > Invalid file extension: %s", filePath.c_str() );
            return false;
        }

        std::string fileExtension = filePath.substr( filePath.find_last_of( '.' ) + 1 );

        DirectX::TexMetadata info{};
        DirectX::ScratchImage image{};

        std::wstring filePathW = std::wstring{ filePath.begin(), filePath.end() };

        HRESULT result{ S_OK };
        if ( !_strcmpi( fileExtension.c_str(), "dds" ) )
        {
            result = DirectX::LoadFromDDSFile( filePathW.c_str(), DirectX::DDS_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::LoadTexture > Loading from DDS file failed: %ls",
                    GetDirectX11ErrorMessage( result ) );
                return false;
            }
        }
        else if ( !_strcmpi( fileExtension.c_str(), "tga" ) )
        {
            result = DirectX::LoadFromTGAFile( filePathW.c_str(), &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::LoadTexture > Loading from TGA file failed: %ls",
                    GetDirectX11ErrorMessage( result ) );
                return false;
            }
        }
        else
        {
            result = DirectX::LoadFromWICFile( filePathW.c_str(), DirectX::WIC_FLAGS_NONE, &info, image );
            if ( FAILED( result ) )
            {
                SM_LOG_ERROR( "DirectX11Texture2D::LoadTexture > Loading from WIC file failed: %ls",
                    GetDirectX11ErrorMessage( result ) );
                return false;
            }
        }

        result = DirectX::CreateTexture( m_pDirectX11Context->GetDevice(),
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            &m_pTexture );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR(
                "DirectX11Texture2D::LoadTexture > Failed to create texture: %ls", GetDirectX11ErrorMessage( result ) );
            SAFE_RELEASE( m_pTexture );
            return false;
        }

        result = DirectX::CreateShaderResourceView( m_pDirectX11Context->GetDevice(),
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            &m_pShaderResourceView );
        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DirectX11Texture2D::LoadTexture > Failed to create shader resource view: %ls",
                GetDirectX11ErrorMessage( result ) );
            SAFE_RELEASE( m_pShaderResourceView );
            return false;
        }

        return true;
    }

    DirectX11Texture2D::~DirectX11Texture2D()
    {
        SAFE_RELEASE( m_pTexture );
        SAFE_RELEASE( m_pShaderResourceView );
    }
}