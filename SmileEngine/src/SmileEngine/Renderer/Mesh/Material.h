#pragma once

#include "SmileEngine/Renderer/Shader/Shader.h"
#include "SmileEngine/Renderer/Resource/Texture.h"

namespace smile
{
    class Material final
    {
      public:
        Material( bool bSkinned = false );
        virtual ~Material() = default;

        void SetAlbedo( const DirectX::XMFLOAT3 &albedo );
        void SetAlbedo( const Ref< Texture2D > &pAlbedoMap );

        void SetMetalness( float metalness );
        void SetMetalness( const Ref< Texture2D > &pMetalnessMap );

        void SetRoughness( float roughness );
        void SetRoughness( const Ref< Texture2D > &pRoughnessMap );

        void SetNormalMap( const Ref< Texture2D > &pNormalMap );
        void SetAOMap( const Ref< Texture2D > &pAOMap );

        const BufferLayout &GetBufferLayout() const
        {
            return m_pShader->GetBufferLayout();
        }
        const Ref< Shader > &GetShader() const
        {
            return m_pShader;
        }

        void SetUseAlbedoMap( bool bUse );
        void SetUseMetalnessMap( bool bUse );
        void SetUseRoughnessMap( bool bUse );
        void SetUseNormalMap( bool bUse );
        void SetUseAOMap( bool bUse );

        const DirectX::XMFLOAT3 &GetAlbedoColor() const
        {
            return m_Albedo;
        }
        float GetMetalness() const
        {
            return m_Metalness;
        }
        float GetRoughness() const
        {
            return m_Roughness;
        }

        bool GetUseAlbedoMap() const
        {
            return m_bUseAlbedoMap;
        }
        bool GetUseMetalnessMap() const
        {
            return m_bUseMetalnessMap;
        }
        bool GetUseRoughnessMap() const
        {
            return m_bUseRoughnessMap;
        }
        bool GetUseNormalMap() const
        {
            return m_bUseNormalMap;
        }
        bool GetUseAOMap() const
        {
            return m_bUseAOMap;
        }

        const Ref< Texture2D > &GetAlbedoMap() const
        {
            return m_pAlbedoMap;
        }
        const Ref< Texture2D > &GetMetalnessMap() const
        {
            return m_pMetalnessMap;
        }
        const Ref< Texture2D > &GetRoughnessMap() const
        {
            return m_pRoughnessMap;
        }
        const Ref< Texture2D > &GetNormalMap() const
        {
            return m_pNormalMap;
        }
        const Ref< Texture2D > &GetAOMap() const
        {
            return m_pAOMap;
        }

      private:
        DirectX::XMFLOAT3 m_Albedo = { 0.f, 0.f, 0.f };
        float m_Metalness = 0.0f;
        float m_Roughness = 0.5f;

        Ref< Texture2D > m_pAlbedoMap = nullptr;
        Ref< Texture2D > m_pMetalnessMap = nullptr;
        Ref< Texture2D > m_pRoughnessMap = nullptr;
        Ref< Texture2D > m_pNormalMap = nullptr;
        Ref< Texture2D > m_pAOMap = nullptr;

        bool m_bUseAlbedoMap = false;
        bool m_bUseMetalnessMap = false;
        bool m_bUseRoughnessMap = false;
        bool m_bUseNormalMap = false;
        bool m_bUseAOMap = false;

        BufferLayout m_BufferLayout;
        Ref< Shader > m_pShader = nullptr;
    };
}
