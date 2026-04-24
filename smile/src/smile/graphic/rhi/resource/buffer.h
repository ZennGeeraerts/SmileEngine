/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/hash_code.h"
#include "smile/common/foundation/flags.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "smile/common/primitive/handle_manager.h"
#include "smile/graphic/rhi/format.h"
#include "smile/graphic/rhi/cpu_access_mode.h"
#include "smile/graphic/rhi/shader/resource_type.h"

namespace smile::graphic::rhi
{
    struct BufferElement final
    {
        BufferElement() = default;

        BufferElement( Format format, const primitive::String &name )
            : Name{ name }, FormatType{ format }, Size{ GetFormatInfo( format ).BytesPerBlock }, Offset{ 0 }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = std::hash< primitive::String >{}( Name );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( FormatType ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( Size ) );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( Offset ) );
            return hash;
        }

        inline bool operator==( const BufferElement &other ) const noexcept
        {
            return Name == other.Name && FormatType == other.FormatType && Size == other.Size && Offset == other.Offset;
        }

        inline bool operator!=( const BufferElement &other ) const noexcept
        {
            return !( *this == other );
        }

        primitive::String Name;
        Format FormatType;
        Uint8 Size;
        Uint8 Offset;
    };

    class BufferLayout final
    {
      public:
        BufferLayout() = default;

        BufferLayout( std::initializer_list< BufferElement > elements ) : m_Elements{ elements }
        {
            CalculateOffsetAndStride();
        }

        const primitive::Vector< BufferElement > &GetElements() const
        {
            return m_Elements;
        }

        Uint32 GetStride() const
        {
            return m_Stride;
        }

        primitive::Vector< BufferElement >::Iterator begin()
        {
            return m_Elements.begin();
        }
        
        primitive::Vector< BufferElement >::Iterator end()
        {
            return m_Elements.end();
        }

        primitive::Vector< BufferElement >::ConstIterator begin() const
        {
            return m_Elements.begin();
        }

        primitive::Vector< BufferElement >::ConstIterator end() const
        {
            return m_Elements.end();
        }

        void AddElement( const BufferElement &element )
        {
            m_Elements.PushBack( element );
            CalculateOffsetAndStride();
        }

        bool ContainsElement( const BufferElement &element ) const
        {
            return primitive::array::HasItem( m_Elements, element );
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;

            for ( const BufferElement &elem : m_Elements )
                hash = foundation::HashCombine( hash, elem.GetHashCode() );

            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( m_Stride ) );

            return hash;
        }

        inline bool operator==( const BufferLayout &other ) const
        {
            if ( m_Stride != other.m_Stride || m_Elements.GetItemCount() != other.m_Elements.GetItemCount() )
                return false;

            return std::equal( m_Elements.begin(), m_Elements.end(), other.m_Elements.begin() );
        }

        inline bool operator!=( const BufferLayout &other ) const
        {
            return !( *this == other );
        }

      private:
        void CalculateOffsetAndStride()
        {
            Uint8 offset{ 0 };
            m_Stride = 0;
            for ( auto &element : m_Elements )
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

      private:
        primitive::Vector< BufferElement > m_Elements;
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

        constexpr BufferRange( Uint32 offset, Uint32 size ) noexcept : Offset{ offset }, Size{ size }
        {
        }

        BufferRange Resolve( const GPUBufferDescriptor &bufferDesc ) const;

        Uint32 Offset;
        Uint32 Size;
    };

    constexpr static BufferRange s_EntireBuffer{ 0, std::numeric_limits< Uint32 >::max() };

    struct BufferBindingKey final : public BufferRange
    {
        BufferBindingKey() = default;

        BufferBindingKey( const BufferRange &range, Format format, ResourceType type )
            : BufferRange{ range }, Format{ format }, Type{ type }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< smile::graphic::rhi::Format >{}( Format ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Offset ) );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( Size ) );
            return hash;
        }

        inline bool operator==( const BufferBindingKey &other ) const
        {
            return Format == other.Format && Type == other.Type && Offset == other.Offset && Size == other.Size;
        }

        Format Format;
        ResourceType Type;
    };

    using GPUBufferHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using GPUBufferHandle = GPUBufferHandleManager::HandleType;

    static constexpr Uint16 s_MaxBufferCount = ( 12 << 10 );
    static constexpr Uint16 s_MaxVertexAttributeCount = 16;
    static constexpr Uint32 s_ConstantBufferOffsetSizeAlignment = 256;

    struct VertexBufferBinding final
    {
        GPUBufferHandle VertexBuffer;
        Uint32 Slot;
        Uint64 Offset;

        inline bool operator==( const VertexBufferBinding &other ) const
        {
            return VertexBuffer == other.VertexBuffer && Slot == other.Slot && Offset == other.Offset;
        }

        inline bool operator!=( const VertexBufferBinding &other ) const
        {
            return !( *this == other );
        }
    };

    struct IndexBufferBinding final
    {
        GPUBufferHandle IndexBuffer;
        Format BufferFormat;
        Uint32 Offset;

        inline bool operator==( const IndexBufferBinding &other ) const
        {
            return IndexBuffer == other.IndexBuffer && BufferFormat == other.BufferFormat && Offset == other.Offset;
        }

        inline bool operator!=( const IndexBufferBinding &other ) const
        {
            return !( *this == other );
        }
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::BufferLayout >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::BufferLayout &bufferLayout ) const noexcept
        {
            return bufferLayout.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::rhi::BufferBindingKey >
    {
        smile::foundation::HashCode operator()(
            const smile::graphic::rhi::BufferBindingKey &bufferBindingKey ) const noexcept
        {
            return bufferBindingKey.GetHashCode();
        }
    };
}