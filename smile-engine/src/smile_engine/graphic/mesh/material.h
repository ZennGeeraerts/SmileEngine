/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile_engine/graphic/renderer_api/shader/shader.h"
#include "smile_engine/graphic/renderer_api/shader/shader_reflection.h"
#include "smile_engine/graphic/renderer_api/resource/texture.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class Material final
    {
      public:
        Material( const Ref< Shader > &pShader );
        ~Material();

        void SetShader( const Ref< Shader > &pShader );

        const BufferLayout &GetBufferLayout() const
        {
            return m_pShader->BufferLayout;
        }
        const Ref< Shader > &GetShader() const
        {
            return m_pShader;
        }

        void SetFloatValue( const std::string &semantic, float value );
        void SetIntValue( const std::string &semantic, int value );
        void SetBoolValue( const std::string &semantic, bool value );
        void SetFloat2Value( const std::string &semantic, const DirectX::XMFLOAT2 &value );
        void SetFloat3Value( const std::string &semantic, const DirectX::XMFLOAT3 &value );
        void SetTexture2D( const std::string &semantic, const Ref< Texture > &value );

        float GetFloatValue( const std::string &semantic ) const;
        int GetIntValue( const std::string &semantic ) const;
        bool GetBoolValue( const std::string &semantic ) const;
        const DirectX::XMFLOAT2 &GetFloat2Value( const std::string &semantic ) const;
        const DirectX::XMFLOAT3 &GetFloat3Value( const std::string &semantic ) const;

        const std::unordered_map< std::string, float > &GetFloatValues() const
        {
            return m_FloatValues;
        }
        const std::unordered_map< std::string, int > &GetIntValues() const
        {
            return m_IntValues;
        }
        const std::unordered_map< std::string, bool > &GetBoolValues() const
        {
            return m_BoolValues;
        }
        const std::unordered_map< std::string, DirectX::XMFLOAT2 > &GetFloat2Values() const
        {
            return m_Float2Values;
        }
        const std::unordered_map< std::string, DirectX::XMFLOAT3 > &GetFloat3Values() const
        {
            return m_Float3Values;
        }
        const std::unordered_map< std::string, Ref< Texture > > &GetTexture2DValues() const
        {
            return m_Texture2DValues;
        }

      private:
        std::unordered_map< std::string, float > m_FloatValues{};
        std::unordered_map< std::string, int > m_IntValues{};
        std::unordered_map< std::string, bool > m_BoolValues{};
        std::unordered_map< std::string, DirectX::XMFLOAT2 > m_Float2Values{};
        std::unordered_map< std::string, DirectX::XMFLOAT3 > m_Float3Values{};
        std::unordered_map< std::string, Ref< Texture > > m_Texture2DValues{};

        Ref< Shader > m_pShader = nullptr;
    };
}
