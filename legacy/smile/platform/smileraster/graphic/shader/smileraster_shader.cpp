/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smileraster_shader.h"

namespace smile::graphic
{
    void SmileRasterShader::UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix )
    {
        pDeviceContext->UploadMat4( sementicName, matrix );
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
        pDeviceContext->UploadTexture2D( sementicName, textureID );
    }

    void SmileRasterShader::UploadTextureCube( const std::string &sementicName, const Ref< TextureCube > &pTextureCube )
    {
    }

    void SmileRasterShader::UploadBool( const std::string &sementicName, bool value )
    {
    }

    void SmileRasterShader::UploadFloat( const std::string &sementicName, float value )
    {
    }
}