#pragma once

#include "smile_engine/renderer/resource/texture.h"
#include "platform/directX11/directx11_context.h"

#include <DirectXTex.h>

namespace smile::renderer
{
    class DirectX11Texture2D final : public Texture2D
    {
      public:
        DirectX11Texture2D( const std::string &file_path );
        virtual ~DirectX11Texture2D();

        DirectX11Texture2D( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D( DirectX11Texture2D && ) = delete;
        DirectX11Texture2D &operator=( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D &operator=( DirectX11Texture2D && ) = delete;

        virtual const std::string &getFilePath() const override
        {
            return filePath;
        }
        virtual Uint32 getWidth() const override
        {
            return width;
        }
        virtual Uint32 getHeight() const override
        {
            return height;
        }
        virtual void *getData() const override
        {
            return shaderResourceView;
        }

      private:
        bool loadTexture( const std::string &file_path );

      private:
        std::string filePath;
        Uint32 width = 0;
        Uint32 height = 0;

        DirectX11Context *directX11Context = nullptr;
        ID3D11Resource *texture = nullptr;
        ID3D11ShaderResourceView *shaderResourceView = nullptr;
    };
}
