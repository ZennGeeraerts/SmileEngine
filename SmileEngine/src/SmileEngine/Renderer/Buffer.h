#pragma once
#include "SmileEngine/Core/Logger.h"

namespace smile
{
    class GraphicsContext;

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
        Bool
    };

    static Uint32 ShaderDataTypeSize( ShaderDataType type )
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
                SM_ASSERT( false, "Buffer.h > ShaderDataTypeSize > Unknown ShaderDataType" );
                return 0;
        }
    }

    struct BufferElement final
    {
        BufferElement() = default;
        BufferElement( ShaderDataType type, const std::string &name, bool bNorm = false )
            : m_Name{ name }, m_DataType{ type }, m_Size{ ShaderDataTypeSize( type ) }, m_Offset{ 0 }, m_bNormalized{ bNorm }
        {
        }

        uint32_t GetComponentCount() const
        {
            switch ( m_DataType )
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
                    SM_ASSERT( false, "BufferElement::GetElementCount > Unknown ShaderDataType" );
                    return 0;
            }
        }

        std::string m_Name;
        ShaderDataType m_DataType;
        uint32_t m_Size;
        uint32_t m_Offset;
        bool m_bNormalized;
    };

    class BufferLayout final
    {
      public:
        BufferLayout()
        {
        }
        BufferLayout( const std::initializer_list< BufferElement > &elements ) : m_Elements{ elements }
        {
            CalculateOffsetAndStride();
        }

        inline const std::vector< BufferElement > &GetElements() const
        {
            return m_Elements;
        }
        inline uint32_t GetStride() const
        {
            return m_Stride;
        }

        std::vector< BufferElement >::iterator begin()
        {
            return m_Elements.begin();
        }
        std::vector< BufferElement >::iterator end()
        {
            return m_Elements.end();
        }
        std::vector< BufferElement >::const_iterator begin() const
        {
            return m_Elements.cbegin();
        }
        std::vector< BufferElement >::const_iterator end() const
        {
            return m_Elements.cend();
        }

      private:
        void CalculateOffsetAndStride()
        {
            uint32_t offset{ 0 };
            m_Stride = 0;
            for ( auto &element : m_Elements )
            {
                element.m_Offset = offset;
                offset += element.m_Size;
                m_Stride += element.m_Size;
            }
        }

      private:
        std::vector< BufferElement > m_Elements;
        Uint32 m_Stride = 0;
    };

    enum class BufferUsage : Uint8
    {
        Default = 0,
        Immutable,
        Dynamic,
        Staging
    };

    struct VertexBufferData final
    {
        void *m_pVertices = nullptr;
        Uint32 m_Count = 0;
        BufferLayout m_BufferLayout = { { ShaderDataType::Float3, "Position" } };
        BufferUsage m_Usage = BufferUsage::Default;
    };

    struct IndexBufferData final
    {
        Uint32 *m_pIndices = nullptr;
        Uint32 m_Count = 0;
        BufferUsage m_Usage = BufferUsage::Default;
    };

    class VertexBuffer
    {
      public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout( const BufferLayout &layout ) = 0;
        virtual const BufferLayout &GetLayout() const = 0;

        static VertexBuffer *Create( const VertexBufferData &vertexBufferData );
    };

    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;

        static IndexBuffer *Create( const IndexBufferData &indexBufferData );
    };
}