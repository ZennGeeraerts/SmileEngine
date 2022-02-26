#pragma once
#include "SmileEngine/Renderer/Shader.h"
#include "SmileEngine/Renderer/Buffer.h"

#include "DirectX11Context.h"
#include <d3dx11effect.h>

namespace smile
{
    class DirectX11Shader final : public Shader
    {
      public:
        DirectX11Shader( const std::string &assetFile,
            const BufferLayout &layout,
            const std::string &techniqueName = "" );

        DirectX11Shader( const std::string &assetFile, const std::string &techniqueName = "" );

        virtual ~DirectX11Shader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const std::string &GetName() const override
        {
            return m_Name;
        }

        virtual const BufferLayout &GetBufferLayout() const override
        {
            return m_BufferLayout;        
        }

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) override;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) override;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) override;
        virtual void UploadInt( const std::string &sementicName, int value ) override;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) override;
        virtual void UploadBool( const std::string &sementicName, bool value ) override;
        virtual void UploadFloat( const std::string &sementicName, float value ) override;

        inline ID3DX11Effect *GetEffect() const
        {
            return m_pEffect;
        }
        inline ID3DX11EffectTechnique *GetTechnique() const
        {
            return m_pTechnique;
        }

      private:
        void Initalize(const std::string& assetFile, const std::string& techniqueName);
        void SetName( const std::string &assetFile );
        bool LoadEffect( ID3D11Device *pDevice, const std::string &assetFile );

        void BuildInputLayout( const BufferLayout &layout );
        void BuildInputLayout();
        DXGI_FORMAT ShaderDataTypeToDirectXBaseType( ShaderDataType type );
        ShaderDataType DirectXBaseTypeToShaderDataType( DXGI_FORMAT type );
        ID3DX11EffectVariable *GetEffectVariable( const std::string &sementicName );

      private:
        std::string m_Name;
        std::unordered_map< std::string, ID3DX11EffectVariable * > m_EffectVariableMap;
        DirectX11Context *m_pDirectX11Context;

        ID3DX11Effect *m_pEffect;
        ID3DX11EffectTechnique *m_pTechnique;
        ID3D11InputLayout *m_pInputLayout;

        BufferLayout m_BufferLayout{};
    };
}
