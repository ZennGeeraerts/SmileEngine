#pragma once
#include "smile_engine/renderer/shader/shader.h"

namespace smile
{
    class SmileRasterContext;

    class SmileRasterShader final : public Shader
    {
      public:
        SmileRasterShader( const std::string &assetFile, const BufferLayout &layout );

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const std::string &GetName() const override
        {
            return m_Name;
        }

        virtual const BufferLayout &GetBufferLayout() const override
        {
            return m_Layout;        
        }

        virtual void UploadMat4( const std::string &sementicName, const DirectX::XMFLOAT4X4 &matrix ) override;
        virtual void UploadMat4Array( const std::string &sementicName,
            const std::vector< DirectX::XMFLOAT4X4 > &matArray ) override;
        virtual void UploadFloat2( const std::string &sementicName, const DirectX::XMFLOAT2 &value ) override;
        virtual void UploadFloat3( const std::string &sementicName, const DirectX::XMFLOAT3 &value ) override;
        virtual void UploadInt( const std::string &sementicName, int value ) override;
        virtual void UploadTexture2D( const std::string &sementicName, const Ref< Texture2D > &pTexture2D ) override;
        virtual void UploadBool( const std::string &sementicName, bool value ) override;
        virtual void UploadFloat( const std::string &sementicName, float value ) override;

      private:
        SmileRasterContext *m_pSmileRasterContext = nullptr;
        std::string m_Name{};
        BufferLayout m_Layout{};
    };
}
