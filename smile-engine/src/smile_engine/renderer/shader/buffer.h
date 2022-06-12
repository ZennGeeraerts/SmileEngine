#pragma once
#include "smile_engine/core/logger.h"

namespace smile::renderer
{
    enum class ShaderDataType : Uint8
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool,
        Mat4Array,
        Texture,
        Texture2D,
        Texture3D,
        TextureCube,
        String
    };

    static Uint32 shaderDataTypeSize( ShaderDataType type )
    {
        switch ( type )
        {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 8;
            case ShaderDataType::Float3:
                return 12;
            case ShaderDataType::Float4:
                return 16;
            case ShaderDataType::Mat3:
                return 4 * 3 * 3;
            case ShaderDataType::Mat4:
                return 4 * 4 * 4;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 8;
            case ShaderDataType::Int3:
                return 12;
            case ShaderDataType::Int4:
                return 16;
            case ShaderDataType::Bool:
                return 1;
            default:
                SM_ASSERT( false, "Buffer.h > shaderDataTypeSize > Unknown ShaderDataType" );
                return 0;
        }
    }

    struct BufferElement final
    {
        BufferElement() = default;
        BufferElement( ShaderDataType type, const std::string &name, bool normalized = false )
            : name{ name }, dataType{ type }, size{ shaderDataTypeSize( type ) }, offset{ 0 }, normalized{ normalized }
        {
        }

        Uint32 getComponentCount() const
        {
            switch ( dataType )
            {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;
                case ShaderDataType::Mat3:
                    return 9;
                case ShaderDataType::Mat4:
                    return 16;
                case ShaderDataType::Int:
                    return 1;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;
                case ShaderDataType::Bool:
                    return 1;
                default:
                    SM_ASSERT( false, "BufferElement::getElementCount > Unknown ShaderDataType" );
                    return 0;
            }
        }

        std::string name;
        ShaderDataType dataType;
        Uint32 size;
        Uint32 offset;
        bool normalized;
    };

    class BufferLayout final
    {
      public:
        BufferLayout()
        {
        }
        BufferLayout( const std::initializer_list< BufferElement > &elements ) : elements{ elements }
        {
            calculateOffsetAndStride();
        }

        inline const std::vector< BufferElement > &getElements() const
        {
            return elements;
        }
        inline Uint32 getStride() const
        {
            return stride;
        }

        std::vector< BufferElement >::iterator begin()
        {
            return elements.begin();
        }
        std::vector< BufferElement >::iterator end()
        {
            return elements.end();
        }
        std::vector< BufferElement >::const_iterator begin() const
        {
            return elements.cbegin();
        }
        std::vector< BufferElement >::const_iterator end() const
        {
            return elements.cend();
        }

        void addElement( const BufferElement &element )
        {
            elements.push_back( element );
            calculateOffsetAndStride();
        }

      private:
        void calculateOffsetAndStride()
        {
            Uint32 offset{ 0 };
            stride = 0;
            for ( auto &element : elements )
            {
                element.offset = offset;
                offset += element.size;
                stride += element.size;
            }
        }

      private:
        std::vector< BufferElement > elements;
        Uint32 stride = 0;
    };

    enum class BufferUsage : Uint8
    {
        Default = 0,
        Immutable,
        Dynamic,
        Staging
    };
}