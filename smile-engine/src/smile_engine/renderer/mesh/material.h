#pragma once

#include "smile_engine/renderer/shader/shader.h"
#include "smile_engine/renderer/shader/shader_reflection.h"
#include "smile_engine/renderer/resource/texture.h"

namespace smile::renderer
{
    class Material final
    {
      public:
        Material( const Ref< Shader > &shader );
        ~Material();

        void setShader( const Ref< Shader > &new_shader );

        const BufferLayout &getBufferLayout() const
        {
            return shader->getBufferLayout();
        }
        const Ref< Shader > &getShader() const
        {
            return shader;
        }

        void setFloatValue( const std::string &semantic, float value );
        void setIntValue( const std::string &semantic, int value );
        void setBoolValue( const std::string &semantic, bool value );
        void setFloat2Value( const std::string &semantic, const DirectX::XMFLOAT2 &value );
        void setFloat3Value( const std::string &semantic, const DirectX::XMFLOAT3 &value );
        void setTexture2D( const std::string &semantic, const Ref< Texture2D > &value );

        float getFloatValue( const std::string &semantic ) const;
        int getIntValue( const std::string &semantic ) const;
        bool getBoolValue( const std::string &semantic ) const;
        const DirectX::XMFLOAT2 &getFloat2Value( const std::string &semantic ) const;
        const DirectX::XMFLOAT3 &getFloat3Value( const std::string &semantic ) const;

        const std::unordered_map< std::string, float > &getFloatValues() const
        {
            return floatValues;
        }
        const std::unordered_map< std::string, int > &getIntValues() const
        {
            return intValues;
        }
        const std::unordered_map< std::string, bool > &getBoolValues() const
        {
            return boolValues;
        }
        const std::unordered_map< std::string, DirectX::XMFLOAT2 > &getFloat2Values() const
        {
            return float2Values;
        }
        const std::unordered_map< std::string, DirectX::XMFLOAT3 > &getFloat3Values() const
        {
            return float3Values;
        }
        const std::unordered_map< std::string, Ref< Texture2D > > &getTexture2DValues() const
        {
            return texture2DValues;
        }

      private:
        std::unordered_map< std::string, float > floatValues{};
        std::unordered_map< std::string, int > intValues{};
        std::unordered_map< std::string, bool > boolValues{};
        std::unordered_map< std::string, DirectX::XMFLOAT2 > float2Values{};
        std::unordered_map< std::string, DirectX::XMFLOAT3 > float3Values{};
        std::unordered_map< std::string, Ref< Texture2D > > texture2DValues{};

        Ref< Shader > shader = nullptr;
    };
}
