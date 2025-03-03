/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "material.h"

#include "engine/graphic/renderer_backend/shader/shader_reflection.h"

namespace smile::graphic
{
    Material::Material( const memory::Ref< Shader > &shader ) : m_pShader{ shader }
    {
        SetShader( shader );
    }

    Material::~Material()
    {
        m_FloatValues.clear();
        m_IntValues.clear();
        m_BoolValues.clear();
        m_Float2Values.clear();
        m_Float3Values.clear();
        m_Texture2DValues.clear();
    }

    void Material::SetShader( const memory::Ref< Shader > &pShader )
    {
        m_pShader = pShader;

        const auto &shaderVariables{ utils::ReflectShaderVariables( pShader ) };
        for ( const ShaderVariable &variable : shaderVariables )
        {
            switch ( variable.Type )
            {
                case ShaderDataType::Float:
                    m_FloatValues.insert( std::make_pair( variable.Semantic, 0.0f ) );
                    break;
                case ShaderDataType::Int:
                    m_IntValues.insert( std::make_pair( variable.Semantic, 0 ) );
                    break;
                case ShaderDataType::Bool:
                    m_BoolValues.insert( std::make_pair( variable.Semantic, false ) );
                    break;
                case ShaderDataType::Float2:
                    m_Float2Values.insert( std::make_pair( variable.Semantic, DirectX::XMFLOAT2{} ) );
                    break;
                case ShaderDataType::Float3:
                    m_Float3Values.insert( std::make_pair( variable.Semantic, DirectX::XMFLOAT3{} ) );
                    break;
                case ShaderDataType::Texture2D:
                    m_Texture2DValues.insert( std::make_pair( variable.Semantic, nullptr ) );
                    break;
            }
        }
    }

    void Material::SetFloatValue( const std::string &semantic, float value )
    {
        if ( m_FloatValues.find( semantic ) == m_FloatValues.end() )
        {
            SM_LOG_WARNING( "Material::SetFloatValue > Couldn't find semantic: {}", semantic.c_str() );
            return;
        }

        m_FloatValues[semantic] = value;
        m_pShader->UploadFloat( semantic, value );
    }

    void Material::SetIntValue( const std::string &semantic, int value )
    {
        if ( m_IntValues.find( semantic ) == m_IntValues.end() )
        {
            SM_LOG_WARNING( "Material::SetIntValue > Couldn't find semantic: {}", semantic );
            return;
        }

        m_IntValues[semantic] = value;
        m_pShader->UploadInt( semantic, value );
    }

    void Material::SetBoolValue( const std::string &semantic, bool value )
    {
        if ( m_BoolValues.find( semantic ) == m_BoolValues.end() )
        {
            SM_LOG_WARNING( "Material::setBoolValue > Couldn't find semantic: {}", semantic );
            return;
        }

        m_BoolValues[semantic] = value;
        m_pShader->UploadBool( semantic, value );
    }

    void Material::SetFloat2Value( const std::string &semantic, const DirectX::XMFLOAT2 &value )
    {
        if ( m_Float2Values.find( semantic ) == m_Float2Values.end() )
        {
            SM_LOG_WARNING( "Material::SetFloat2Value > Couldn't find semantic: {}", semantic );
            return;
        }

        m_Float2Values[semantic] = value;
        m_pShader->UploadFloat2( semantic, value );
    }

    void Material::SetFloat3Value( const std::string &semantic, const DirectX::XMFLOAT3 &value )
    {
        if ( m_Float3Values.find( semantic ) == m_Float3Values.end() )
        {
            SM_LOG_WARNING( "Material::SetFloat3Value > Couldn't find semantic: {}", semantic );
            return;
        }

        m_Float3Values[semantic] = value;
        m_pShader->UploadFloat3( semantic, value );
    }

    void Material::SetTexture2D( const std::string &semantic, const memory::Ref< Texture > &value )
    {
        if ( m_Texture2DValues.find( semantic ) == m_Texture2DValues.end() )
        {
            SM_LOG_WARNING( "Material::SetTexture2D > Couldn't find semantic: {}", semantic );
            return;
        }

        m_Texture2DValues[semantic] = value;
        m_pShader->UploadTexture( semantic, value );
    }

    float Material::GetFloatValue( const std::string &semantic ) const
    {
        auto it = m_FloatValues.find( semantic );
        SM_ASSERT( it != m_FloatValues.end(), "Material::GetFloatValue > Couldn't find semantic: %s", semantic );

        return it->second;
    }

    int Material::GetIntValue( const std::string &semantic ) const
    {
        auto it = m_IntValues.find( semantic );
        SM_ASSERT( it != m_IntValues.end(), "Material::GetFloatValue > Couldn't find semantic: %s", semantic );

        return it->second;
    }

    bool Material::GetBoolValue( const std::string &semantic ) const
    {
        auto it = m_BoolValues.find( semantic );
        SM_ASSERT( it != m_BoolValues.end(), "Material::GetBoolValue > Couldn't find semantic: %s", semantic );

        return it->second;
    }

    const DirectX::XMFLOAT2 &Material::GetFloat2Value( const std::string &semantic ) const
    {
        auto it = m_Float2Values.find( semantic );
        SM_ASSERT( it != m_Float2Values.end(), "Material::GetFloat2Value > Couldn't find semantic: %s", semantic );

        return it->second;
    }

    const DirectX::XMFLOAT3 &Material::GetFloat3Value( const std::string &semantic ) const
    {
        auto it = m_Float3Values.find( semantic );
        SM_ASSERT( it != m_Float3Values.end(), "Material::GetFloat3Value > Couldn't find semantic: %s", semantic );

        return it->second;
    }
}