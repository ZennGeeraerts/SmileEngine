#include "smpch.h"
#include "smileraster_shader.h"

#include "smileraster_context.h"
#include "smile_engine/core/application.h"

namespace smile::graphic
{
    SmileRasterShader::SmileRasterShader( const std::string &asset_file, const BufferLayout &layout )
        : bufferLayout{ layout }
    {
        smileRasterContext =
            static_cast< SmileRasterContext * >( Application::getInstance().getWindow().getGraphicsContext() );
        SM_ASSERT(
            smileRasterContext, "SmileRasterShader > Rendering context is not a SmileRaster Rendering Context" );

        // Find name from asset path
        auto last_slash = asset_file.find_last_of( "/\\" );
        last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
        auto last_dot = asset_file.rfind( '.' );
        auto count = last_dot == std::string::npos ? asset_file.size() - last_slash : last_dot - last_slash;
        name = asset_file.substr( last_slash, count );
    }

    void SmileRasterShader::bind() const
    {
    }

    void SmileRasterShader::unbind() const
    {
    }

    void SmileRasterShader::uploadMat4( const std::string &sementic_name, const DirectX::XMFLOAT4X4 &matrix )
    {
        smileRasterContext->getDeviceContext()->UploadMat4( sementic_name, matrix );
    }

    void SmileRasterShader::uploadMat4Array( const std::string &sementic_name,
        const std::vector< DirectX::XMFLOAT4X4 > &mat_array )
    {
    }

    void SmileRasterShader::uploadFloat2( const std::string &sementic_name, const DirectX::XMFLOAT2 &value )
    {
    }

    void SmileRasterShader::uploadFloat3( const std::string &sementic_name, const DirectX::XMFLOAT3 &value )
    {
    }

    void SmileRasterShader::uploadInt( const std::string &sementic_name, int value )
    {
    }

    void SmileRasterShader::uploadTexture2D( const std::string &sementic_name, const Ref< Texture2D > &texture_2d )
    {
        Raster::TextureID textureID = reinterpret_cast< uint32_t >( texture_2d->getData() );
        smileRasterContext->getDeviceContext()->UploadTexture2D( sementic_name, textureID );
    }

    void SmileRasterShader::uploadBool( const std::string &sementic_name, bool value )
    {
    }

    void SmileRasterShader::uploadFloat( const std::string &sementic_name, float value )
    {
    }
}