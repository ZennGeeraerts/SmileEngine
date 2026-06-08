/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/flags.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "smile/common/primitive/collection/fixed_vector.h"
#include "smile/common/primitive/handle_manager.h"
#include "resource_type.h"
#include "shader_stage.h"

namespace smile::graphic::rhi
{
    struct BindingLayoutElement final
    {
        BindingLayoutElement() = default;

        BindingLayoutElement( Uint32 slot, ResourceType type ) noexcept : Slot{ slot }, Type{ type }, Size{ 0 }
        {
        }

        BindingLayoutElement( Uint32 slot, ResourceType type, Uint16 size ) noexcept
            : Slot{ slot }, Type{ type }, Size{ size }
        {
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< Uint32 >{}( Slot );
            hash = foundation::HashCombine( hash, std::hash< Uint8 >{}( static_cast< Uint8 >( Type ) ) );
            hash = foundation::HashCombine( hash, std::hash< Uint16 >{}( Size ) );
            return hash;
        }

        bool operator==( const BindingLayoutElement &other ) const noexcept
        {
            return Slot == other.Slot && Type == other.Type && Size == other.Size;
        }

        bool operator!=( const BindingLayoutElement &other ) const noexcept
        {
            return !( *this == other );
        }

        Uint32 Slot;
        ResourceType Type;
        Uint16 Size;
    };

    class BindingLayout final
    {
      public:
        BindingLayout() = default;

        BindingLayout( foundation::Flags< ShaderStage > visibility ) noexcept : m_Visibility{ visibility }
        {
        }

        BindingLayout( foundation::Flags< ShaderStage > visibility,
            std::initializer_list< BindingLayoutElement > elements ) noexcept
            : m_Visibility{ visibility }, m_Elements{ elements }
        {
        }

        const primitive::ArrayView< const BindingLayoutElement > GetElements() const noexcept
        {
            return m_Elements.AsView();
        }

        foundation::Flags< ShaderStage > GetVisibility() const noexcept
        {
            return m_Visibility;
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::hash< primitive::Vector< BindingLayoutElement > >{}( m_Elements );
            hash = foundation::HashCombine( hash, std::hash< Uint32 >{}( m_Visibility.GetFlags() ) );
            return hash;
        }

        primitive::Vector< BindingLayoutElement >::Iterator begin() noexcept
        {
            return m_Elements.begin();
        }

        primitive::Vector< BindingLayoutElement >::Iterator end() noexcept
        {
            return m_Elements.end();
        }

        primitive::Vector< BindingLayoutElement >::ConstIterator begin() const noexcept
        {
            return m_Elements.begin();
        }

        primitive::Vector< BindingLayoutElement >::ConstIterator end() const noexcept
        {
            return m_Elements.end();
        }

        void AddElement( const BindingLayoutElement &element )
        {
            m_Elements.PushBack( element );
        }

        void AddElement( BindingLayoutElement &&element )
        {
            m_Elements.EmplaceBack( std::move( element ) );
        }

        bool HasElement( const BindingLayoutElement &element ) const noexcept
        {
            return primitive::array::HasItem( m_Elements, element );
        }

        void Clear() noexcept
        {
            m_Elements.Clear();
        }

        bool operator==( const BindingLayout &other ) const noexcept
        {
            return m_Visibility == other.m_Visibility && primitive::array::IsEqual( m_Elements, other.m_Elements );
        }

        bool operator!=( const BindingLayout &other ) const noexcept
        {
            return !( *this == other );
        }

      private:
        primitive::Vector< BindingLayoutElement > m_Elements;
        foundation::Flags< ShaderStage > m_Visibility;
    };

    using BindingLayoutHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using BindingLayoutHandle = BindingLayoutHandleManager::HandleType;

    static constexpr Uint16 s_MaxBindingLayoutCount = 5;

    using BindingLayoutVector = primitive::FixedVector< BindingLayoutHandle, s_MaxBindingLayoutCount >;
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::BindingLayoutElement >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::BindingLayoutElement &element ) const
        {
            return element.GetHashCode();
        }
    };

    template <>
    struct hash< smile::graphic::rhi::BindingLayout >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::BindingLayout &layout ) const
        {
            return layout.GetHashCode();
        }
    };
}