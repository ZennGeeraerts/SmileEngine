#pragma once

#include "smile_engine/core/core.h"
#include "buffer.h"
#include "smile_engine/renderer/resource/texture.h"

namespace smile
{
    class Shader
    {
      public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual const std::string &GetName() const = 0;
        virtual const BufferLayout &GetBufferLayout() const = 0;

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) = 0;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) = 0;
        virtual void UploadFloat2( const std::string &semanticName, const DirectX::XMFLOAT2 &value ) = 0;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) = 0;
        virtual void UploadInt( const std::string &sementicName, int value ) = 0;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) = 0;
        virtual void UploadBool( const std::string &sementicName, bool value ) = 0;
        virtual void UploadFloat( const std::string &sementicName, float value ) = 0;

        static Ref< Shader > Create( const std::string &filePath, const BufferLayout &bufferLayout );
        static Ref< Shader > Create( const std::string &filePath );
    };
}
