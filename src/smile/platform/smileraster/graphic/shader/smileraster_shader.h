/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/shader/shader.h"

#include <device_context.cuh>

namespace smile::graphic
{
    class SmileRasterContext;

    struct SmileRasterShader final : public Shader
    {
        SmileRasterShader() = default;

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) override;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) override;
        virtual void UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value ) override;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) override;
        virtual void UploadInt( const std::string &sementicName, int value ) override;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) override;
        virtual void UploadTextureCube( const std::string &sementicName, const Ref< TextureCube > &pTextureCube ) override;
        virtual void UploadBool( const std::string &sementicName, bool value ) override;
        virtual void UploadFloat( const std::string &sementicName, float value ) override;

        void* GetData() const override
        {
            return nullptr;
        }

        Raster::DeviceContext *pDeviceContext = nullptr;
    };
}
