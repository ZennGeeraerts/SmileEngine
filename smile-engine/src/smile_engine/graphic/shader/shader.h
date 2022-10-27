#pragma once

#include "smile_engine/core/core.h"
#include "buffer.h"
#include "smile_engine/graphic/resource/texture.h"

namespace smile::graphic
{
    class Shader
    {
      public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const std::string &getName() const = 0;
        virtual const BufferLayout &getBufferLayout() const = 0;

        virtual void uploadMat4( const std::string &sementic_name, const DirectX::XMFLOAT4X4 &matrix ) = 0;
        virtual void uploadMat4Array( const std::string &sementic_name,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) = 0;
        virtual void uploadFloat2( const std::string &sementic_name, const DirectX::XMFLOAT2 &value ) = 0;
        virtual void uploadFloat3( const std::string &sementic_name, const DirectX::XMFLOAT3 &value ) = 0;
        virtual void uploadInt( const std::string &sementic_name, int value ) = 0;
        virtual void uploadTexture2D( const std::string &sementic_name, const Ref< Texture2D > &texture_2d ) = 0;
        virtual void uploadBool( const std::string &sementic_name, bool value ) = 0;
        virtual void uploadFloat( const std::string &sementic_name, float value ) = 0;

        static Ref< Shader > create( const std::string &file_path, const BufferLayout &buffer_layout );
        static Ref< Shader > create( const std::string &file_path );
    };
}
