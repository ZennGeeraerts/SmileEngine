#pragma once

#include "smile_engine/graphic/resource/texture.h"
#include "platform/directX11/directx11_context.h"

#include <DirectXTex.h>

namespace smile::graphic
{
    class DirectX11Texture2D final : public Texture2D
    {
      public:
        DirectX11Texture2D( const std::string &filePath );
        virtual ~DirectX11Texture2D();

        DirectX11Texture2D( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D( DirectX11Texture2D && ) = delete;
        DirectX11Texture2D &operator=( const DirectX11Texture2D & ) = delete;
        DirectX11Texture2D &operator=( DirectX11Texture2D && ) = delete;

        virtual const std::string &GetFilePath() const override
        {
            return m_FilePath;
        }
        virtual Uint32 GetWidth() const override
        {
            return m_Width;
        }
        virtual Uint32 GetHeight() const override
        {
            return m_Height;
        }
        virtual void *GetData() const override
        {
            return m_pShaderResourceView;
        }

      private:
        bool LoadTexture( const std::string &filePath );

      private:
        std::string m_FilePath;
        Uint32 m_Width = 0;
        Uint32 m_Height = 0;

        DirectX11Context *m_pDirectX11Context = nullptr;
        ID3D11Resource *m_pTexture = nullptr;
        ID3D11ShaderResourceView *m_pShaderResourceView = nullptr;
    };
}
