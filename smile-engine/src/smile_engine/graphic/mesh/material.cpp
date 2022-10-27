#include "smpch.h"
#include "material.h"

#include "smile_engine/graphic/shader/shader_reflection.h"

namespace smile::graphic
{
    Material::Material( const Ref< Shader > &shader ) : shader{ shader }
    {
        setShader( shader );
    }

    Material::~Material()
    {
        floatValues.clear();
        intValues.clear();
        boolValues.clear();
        float2Values.clear();
        float3Values.clear();
        texture2DValues.clear();
    }

    void Material::setShader( const Ref< Shader > &new_shader )
    {
        shader = new_shader;

        const auto &shader_variables{ utils::reflectShaderVariables( new_shader ) };
        for ( const ShaderVariable &variable : shader_variables )
        {
            switch ( variable.type )
            {
                case ShaderDataType::Float:
                    floatValues.insert( std::make_pair( variable.semantic, 0.0f ) );
                    break;
                case ShaderDataType::Int:
                    intValues.insert( std::make_pair( variable.semantic, 0 ) );
                    break;
                case ShaderDataType::Bool:
                    boolValues.insert( std::make_pair( variable.semantic, false ) );
                    break;
                case ShaderDataType::Float2:
                    float2Values.insert( std::make_pair( variable.semantic, DirectX::XMFLOAT2{} ) );
                    break;
                case ShaderDataType::Float3:
                    float3Values.insert( std::make_pair( variable.semantic, DirectX::XMFLOAT3{} ) );
                    break;
                case ShaderDataType::Texture2D:
                    texture2DValues.insert( std::make_pair( variable.semantic, nullptr ) );
                    break;
            }
        }
    }

    void Material::setFloatValue( const std::string &semantic, float value )
    {
        if ( floatValues.find( semantic ) == floatValues.end() )
        {
            SM_LOG_WARNING( "Material::setFloatValue > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        floatValues[semantic] = value;
        shader->uploadFloat( semantic, value );
    }

    void Material::setIntValue( const std::string &semantic, int value )
    {
        if ( intValues.find( semantic ) == intValues.end() )
        {
            SM_LOG_WARNING( "Material::setIntValue > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        intValues[semantic] = value;
        shader->uploadInt( semantic, value );
    }

    void Material::setBoolValue( const std::string &semantic, bool value )
    {
        if ( boolValues.find( semantic ) == boolValues.end() )
        {
            SM_LOG_WARNING( "Material::setBoolValue > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        boolValues[semantic] = value;
        shader->uploadBool( semantic, value );
    }

    void Material::setFloat2Value( const std::string &semantic, const DirectX::XMFLOAT2 &value )
    {
        if ( float2Values.find( semantic ) == float2Values.end() )
        {
            SM_LOG_WARNING( "Material::setFloat2Value > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        float2Values[semantic] = value;
        shader->uploadFloat2( semantic, value );
    }

    void Material::setFloat3Value( const std::string &semantic, const DirectX::XMFLOAT3 &value )
    {
        if ( float3Values.find( semantic ) == float3Values.end() )
        {
            SM_LOG_WARNING( "Material::setFloat3Value > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        float3Values[semantic] = value;
        shader->uploadFloat3( semantic, value );
    }

    void Material::setTexture2D( const std::string &semantic, const Ref< Texture2D > &value )
    {
        if ( texture2DValues.find( semantic ) == texture2DValues.end() )
        {
            SM_LOG_WARNING( "Material::setTexture2D > Couldn't find semantic: %s", semantic.c_str() );
            return;
        }

        texture2DValues[semantic] = value;
        shader->uploadTexture2D( semantic, value );
    }

    float Material::getFloatValue( const std::string &semantic ) const
    {
        auto it = floatValues.find( semantic );
        SM_ASSERT( it != floatValues.end(), "Material::getFloatValue > Couldn't find semantic: %s", semantic.c_str() );

        return it->second;
    }

    int Material::getIntValue( const std::string &semantic ) const
    {
        auto it = intValues.find( semantic );
        SM_ASSERT( it != intValues.end(), "Material::getFloatValue > Couldn't find semantic: %s", semantic.c_str() );

        return it->second;
    }

    bool Material::getBoolValue( const std::string &semantic ) const
    {
        auto it = boolValues.find( semantic );
        SM_ASSERT( it != boolValues.end(), "Material::getBoolValue > Couldn't find semantic: %s", semantic.c_str() );

        return it->second;
    }

    const DirectX::XMFLOAT2 &Material::getFloat2Value( const std::string &semantic ) const
    {
        auto it = float2Values.find( semantic );
        SM_ASSERT(
            it != float2Values.end(), "Material::getFloat2Value > Couldn't find semantic: %s", semantic.c_str() );

        return it->second;
    }

    const DirectX::XMFLOAT3 &Material::getFloat3Value( const std::string &semantic ) const
    {
        auto it = float3Values.find( semantic );
        SM_ASSERT(
            it != float3Values.end(), "Material::getFloat3Value > Couldn't find semantic: %s", semantic.c_str() );

        return it->second;
    }
}