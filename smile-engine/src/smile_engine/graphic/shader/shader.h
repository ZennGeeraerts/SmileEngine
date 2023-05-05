/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/core/core.h"
#include "buffer.h"
#include "smile_engine/graphic/resource/texture.h"

namespace smile::graphic
{
    struct Shader
    {
        Shader() = default;
        virtual ~Shader() = default;

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) = 0;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) = 0;
        virtual void UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value ) = 0;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) = 0;
        virtual void UploadInt( const std::string &sementicName, int value ) = 0;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) = 0;
        virtual void UploadBool( const std::string &sementicName, bool value ) = 0;
        virtual void UploadFloat( const std::string &sementicName, float value ) = 0;

        virtual void *GetData() const = 0;

        void SetName( const std::string &assetFile );

        std::string Name;
        BufferLayout BufferLayout{};
    };
}
