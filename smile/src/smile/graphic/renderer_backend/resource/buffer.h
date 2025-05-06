/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/graphic/renderer_backend/format.h"

namespace smile::graphic
{
    struct BufferElement final
    {
        BufferElement() = default;
        BufferElement( Format format, const std::string &name )
            : Name{ name }, FormatType{ format }, Size{ GetFormatInfo( format ).BytesPerBlock }, Offset{ 0 }
        {
        }

        std::string Name;
        Format FormatType;
        Uint8 Size;
        Uint8 Offset;
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
        inline Uint32 GetStride() const
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

        void AddElement( const BufferElement &element )
        {
            m_Elements.push_back( element );
            CalculateOffsetAndStride();
        }

      private:
        void CalculateOffsetAndStride()
        {
            Uint32 offset{ 0 };
            m_Stride = 0;
            for ( auto &element : m_Elements )
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
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

    enum class BufferCPUAccess : Uint8
    {
        None,
        Read,
        Write
    };

    enum class BufferBindFlags : Uint8
    {
        None = BIT( 0 ),
        VertexBuffer = BIT( 1 ),
        IndexBuffer = BIT( 2 ),
        UniformBuffer = BIT( 3 )
    };
}