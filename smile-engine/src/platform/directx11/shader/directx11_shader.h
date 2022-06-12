#pragma once
#include "smile_engine/renderer/shader/shader.h"
#include "smile_engine/renderer/shader/buffer.h"

#include "platform/directx11/directx11_context.h"

#include <d3dx11effect.h>

namespace smile::renderer
{
    class DirectX11Shader final : public Shader
    {
      public:
        DirectX11Shader( const std::string &asset_file,
            const BufferLayout &layout,
            const std::string &technique_name = "" );

        DirectX11Shader( const std::string &asset_file, const std::string &technique_name = "" );

        virtual ~DirectX11Shader();

        DirectX11Shader( const DirectX11Shader & ) = delete;
        DirectX11Shader( DirectX11Shader && ) = delete;
        DirectX11Shader &operator=( const DirectX11Shader & ) = delete;
        DirectX11Shader &operator=( DirectX11Shader && ) = delete;

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual const std::string &getName() const override
        {
            return name;
        }

        virtual const BufferLayout &getBufferLayout() const override
        {
            return bufferLayout;        
        }

        virtual void uploadMat4( const std::string &sementic_name, const DirectX::XMFLOAT4X4 &matrix ) override;
        virtual void uploadMat4Array( const std::string &sementic_name,
            const std::vector< DirectX::XMFLOAT4X4 > &mat_array ) override;
        virtual void uploadFloat2( const std::string &sementic_name, const DirectX::XMFLOAT2 &value ) override;
        virtual void uploadFloat3( const std::string &sementic_name, const DirectX::XMFLOAT3 &value ) override;
        virtual void uploadInt( const std::string &sementic_name, int value ) override;
        virtual void uploadTexture2D( const std::string &sementic_name, const Ref< Texture2D > &texture_2d ) override;
        virtual void uploadBool( const std::string &sementic_name, bool value ) override;
        virtual void uploadFloat( const std::string &sementic_name, float value ) override;

        inline ID3DX11Effect *getEffect() const
        {
            return effect;
        }
        inline ID3DX11EffectTechnique *getTechnique() const
        {
            return technique;
        }

      private:
        void initalize(const std::string& asset_file, const std::string& technique_name);
        void setName( const std::string &asset_file );
        bool loadEffect( ID3D11Device *device, const std::string &asset_file );

        void buildInputLayout( const BufferLayout &layout );
        void buildInputLayout();
       
        ID3DX11EffectVariable *getEffectVariable( const std::string &sementic_name );

      private:
        std::string name;
        std::unordered_map< std::string, ID3DX11EffectVariable * > effectVariableMap;

        DirectX11Context *directX11Context;

        ID3DX11Effect *effect;
        ID3DX11EffectTechnique *technique;
        ID3D11InputLayout *inputLayout;

        BufferLayout bufferLayout{};
    };
}
