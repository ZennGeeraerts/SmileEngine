#pragma once
#include "smile_engine/graphic/shader/shader.h"

#include <d3dx11effect.h>

namespace smile::graphic
{
    struct DirectX11Shader final : public Shader
    {
        DirectX11Shader() = default;
        virtual ~DirectX11Shader();

        DirectX11Shader( const DirectX11Shader & ) = delete;
        DirectX11Shader( DirectX11Shader && ) = delete;
        DirectX11Shader &operator=( const DirectX11Shader & ) = delete;
        DirectX11Shader &operator=( DirectX11Shader && ) = delete;

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) override;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) override;
        virtual void UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value ) override;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) override;
        virtual void UploadInt( const std::string &sementicName, int value ) override;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) override;
        virtual void UploadBool( const std::string &sementicName, bool value ) override;
        virtual void UploadFloat( const std::string &sementicName, float value ) override;

        void *GetData() const override
        {
            return pInputLayout;
        }

        ID3DX11EffectVariable *GetEffectVariable( const std::string &sementicName );

        std::unordered_map< std::string, ID3DX11EffectVariable * > EffectVariableMap;
        ID3DX11Effect *pEffect = nullptr;
        ID3DX11EffectTechnique *pTechnique = nullptr;
        ID3D11InputLayout *pInputLayout = nullptr;
    };
}
