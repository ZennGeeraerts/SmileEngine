/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/hash_code.h"
#include "smile/common/foundation/flags.h"
#include "smile/graphic/rhi/format.h"
#include "smile/graphic/rhi/cpu_access_mode.h"
#include "smile/graphic/rhi/shader/resource_type.h"

namespace smile::graphic
{
    struct BufferElement final
    {
        BufferElement() = default;
        BufferElement( Format format, const std::string &name )
            : Name{ name }, FormatType{ format }, Size{ GetFormatInfo( format ).BytesPerBlock }, Offset{ 0 }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = std::hash< std::string >{}( Name );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( FormatType ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( Size ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( Offset ) );
            return hash;
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

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;

            for ( const BufferElement &elem : m_Elements )
                hash = foundation::HashCombine( hash, elem.GetHashCode() );

            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( m_Stride ) );

            return hash;
        }

        bool operator()( const BufferLayout &lhs, const BufferLayout &rhs ) const
        {
            return lhs.GetHashCode() == rhs.GetHashCode();
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

    enum class BufferBindFlags : Uint8
    {
        None,
        VertexBuffer,
        IndexBuffer,
        ConstantBuffer,
        ShaderResource,
        UnorderedAccess
    };

    struct GPUBufferDescriptor final
    {
        Uint32 Size;
        BufferUsage Usage = BufferUsage::Default;
        CPUAccessMode CPUAccess = CPUAccessMode::None;
        foundation::Flags< BufferBindFlags > BindFlags{ BufferBindFlags::None };
        Uint32 StructStride = 0;               // If non zero, it is structured
        Format BufferFormat = Format::UNKNOWN; // For typed buffer views
        bool AllowTypedViews = false;
        bool AllowRawViews = false;
    };

    struct BufferRange
    {
        BufferRange() = default;

        constexpr BufferRange( Uint32 offset, Uint32 size ) : Offset{ offset }, Size{ size }
        {
        }

        BufferRange Resolve( const GPUBufferDescriptor &bufferDesc ) const;

        Uint32 Offset;
        Uint32 Size;
    };

    constexpr static BufferRange s_EntireBuffer{ 0, std::numeric_limits< Uint32 >::max() };

    struct BufferBindingKey final : public BufferRange
    {
        BufferBindingKey()
        {
        }

        BufferBindingKey( const BufferRange &range, Format format, ResourceType type )
            : BufferRange{ range }, Format{ format }, Type{ type }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< smile::graphic::Format >{}( Format ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Offset ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Size ) );
            return hash;
        }

        bool operator==( const BufferBindingKey &other ) const
        {
            return Format == other.Format && Type == other.Type && Offset == other.Offset && Size == other.Size;
        }

        Format Format;
        ResourceType Type;
    };

    struct VertexBufferBinding final
    {
        GPUBufferHandle VertexBuffer;
        Uint32 Slot;
        Uint64 Offset;

        bool operator==( const VertexBufferBinding &other ) const
        {
            return VertexBuffer == other.VertexBuffer && Slot == other.Slot && Offset == other.Offset;
        }

        bool operator!=( const VertexBufferBinding &other ) const
        {
            return !( *this == other );
        }
    };

    struct IndexBufferBinding final
    {
        GPUBufferHandle IndexBuffer;
        Format BufferFormat;
        Uint32 Offset;

        bool operator==( const IndexBufferBinding &other ) const
        {
            return IndexBuffer == other.IndexBuffer && BufferFormat == other.BufferFormat && Offset == other.Offset;
        }

        bool operator!=( const IndexBufferBinding &other ) const
        {
            return !( *this == other );
        }
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::BufferLayout >
    {
        smile::foundation::HashCode operator()( const smile::graphic::BufferLayout &bufferLayout ) const noexcept
        {
            return bufferLayout.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::BufferBindingKey >
    {
        smile::foundation::HashCode operator()(
            const smile::graphic::BufferBindingKey &bufferBindingKey ) const noexcept
        {
            return bufferBindingKey.GetHashCode();
        }
    };
}