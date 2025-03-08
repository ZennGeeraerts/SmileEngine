/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
<<<<<<<< HEAD:smile/src/platform/directx11/graphic/shader/directx11_shader.h
#include "smile/graphic/renderer_api/shader/shader.h"
========
#include "engine/graphic/renderer_backend/shader/shader.h"
>>>>>>>> main:src/smile/platform/directx11/graphic/renderer_backend/shader/directx11_shader.h

#include <d3dx11effect.h>

namespace smile::graphic
{
    struct DirectX11Shader final : public Shader
    {
        DirectX11Shader() = default;
        ~DirectX11Shader();

        DirectX11Shader( const DirectX11Shader & ) = delete;
        DirectX11Shader( DirectX11Shader && ) = delete;
        DirectX11Shader &operator=( const DirectX11Shader & ) = delete;
        DirectX11Shader &operator=( DirectX11Shader && ) = delete;

        void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) override;
        void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) override;
        void UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value ) override;
        void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) override;
        void UploadInt( const std::string &sementicName, int value ) override;
        void UploadTexture( const std::string &sementicName, const memory::Ref< Texture > &pTexture ) override;
        void UploadBool( const std::string &sementicName, bool value ) override;
        void UploadFloat( const std::string &sementicName, float value ) override;

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
