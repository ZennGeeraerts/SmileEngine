#include "smpch.h"
#include "smileraster_shader.h"

#include "smileraster_context.h"
#include "smile_engine/core/application.h"

namespace Smile::Graphic
{
    SmileRasterShader::SmileRasterShader( const std::string &asset_file, const BufferLayout &layout )
        : m_BufferLayout{ layout }
    {
        m_pSmileRasterContext =
            static_cast< SmileRasterContext * >( Application::GetInstance().GetWindow().GetGraphicsContext() );
        SM_ASSERT(
            m_pSmileRasterContext, "SmileRasterShader > Rendering context is not a SmileRaster Rendering Context" );

        // Find name from asset path
        auto lastSlash = asset_file.find_last_of( "/\\" );
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = asset_file.rfind( '.' );
        auto count = lastDot == std::string::npos ? asset_file.size() - lastSlash : lastDot - lastSlash;
        m_Name = asset_file.substr( lastSlash, count );
    }

    void SmileRasterShader::Bind() const
    {
    }

    void SmileRasterShader::Unbind() const
    {
    }

    void SmileRasterShader::UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix )
    {
        m_pSmileRasterContext->getDeviceContext()->UploadMat4( sementicName, matrix );
    }

    void SmileRasterShader::UploadMat4Array( const std::string &sementicName,
        const std::vector< DirectX::XMFLOAT4X4 > &matArray )
    {
    }

    void SmileRasterShader::UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value )
    {
    }

    void SmileRasterShader::UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value )
    {
    }

    void SmileRasterShader::UploadInt( const std::string &sementicName, int value )
    {
    }

    void SmileRasterShader::UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D )
    {
        Raster::TextureID textureID = reinterpret_cast< uint32_t >( pTexture2D->GetData() );
        m_pSmileRasterContext->getDeviceContext()->UploadTexture2D( sementicName, textureID );
    }

    void SmileRasterShader::UploadBool( const std::string &sementicName, bool value )
    {
    }

    void SmileRasterShader::UploadFloat( const std::string &sementicName, float value )
    {
    }
}