#pragma once
#include "smile_engine/renderer/shader/shader.h"

namespace smile::renderer
{
    class SmileRasterContext;

    class SmileRasterShader final : public Shader
    {
      public:
        SmileRasterShader( const std::string &asset_file, const BufferLayout &layout );

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

      private:
        SmileRasterContext *smileRasterContext = nullptr;
        std::string name{};
        BufferLayout bufferLayout{};
    };
}
