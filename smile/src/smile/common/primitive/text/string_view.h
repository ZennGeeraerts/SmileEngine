/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/constant_text.h"
#include "smile/common/foundation/numeric_cast.h"
#include "smile/common/memory/memory.h"

#include <format>

namespace smile::primitive
{
    class StringView final
    {
      public:
        StringView() = default;

        constexpr StringView( foundation::ConstantText text ) noexcept
            : m_SubText{ text.GetText() }, m_CharCount{ text.GetCharCount() }
        {
        }

        StringView( const char *text )
            : m_SubText{ text }, m_CharCount{ text ? foundation::NumericCast< Count >( std::strlen( text ) ) : 0 }
        {
        }

        template < int Size >
        constexpr StringView( const char ( &text )[Size] ) noexcept : StringView{ text, Size - 1 }
        {
        }

        constexpr StringView( const char *text, const Count charCount ) noexcept
            : m_SubText{ text }, m_CharCount{ charCount }
        {
        }

        ~StringView() = default;

        constexpr char operator[]( const Index index ) const noexcept
        {
            SM_ASSERT( index < m_CharCount );

            return m_SubText[index];
        }

        constexpr Count GetCharCount() const noexcept
        {
            return m_CharCount;
        }

        constexpr const char *GetData() const noexcept
        {
            SM_ASSERT( IsNullTerminated() );

            return m_SubText;
        }

        constexpr const char *GetSubText() const noexcept
        {
            return m_SubText;
        }

        constexpr bool IsEmpty() const noexcept
        {
            return m_CharCount == 0;
        }

        constexpr bool IsNullTerminated() const
        {
            return m_SubText[m_CharCount] == 0;
        }

        constexpr bool IsValidRange( const Index firstIndex, const Count charCount ) const noexcept
        {
            return ( firstIndex + charCount ) <= m_CharCount;
        }

        constexpr const char *begin() const noexcept
        {
            return m_SubText;
        }

        constexpr const char *end() const noexcept
        {
            return m_SubText + m_CharCount;
        }

        Index Find( const char charToFind, const Index firstCharIndex = 0 ) const;
        Index Find( const StringView textToFind, const Index firstCharIndex = 0 ) const;

      private:
        const char *m_SubText{ nullptr };
        Count m_CharCount{ 0 };
    };

    int Compare( StringView firstText, StringView secondText );

    inline bool operator==( StringView firstView, StringView secondView )
    {
        return firstView.GetCharCount() == secondView.GetCharCount() &&
               memory::CompareByteArrays( firstView.GetSubText(), secondView.GetSubText(), secondView.GetCharCount() );
    }

    inline bool operator!=( StringView firstView, StringView secondView )
    {
        return !( firstView == secondView );
    }

    inline bool operator<( StringView first, StringView second )
    {
        return Compare( first, second ) < 0;
    }

    inline bool operator<=( StringView first, StringView second )
    {
        return Compare( first, second ) <= 0;
    }

    inline bool operator>( StringView first, StringView second )
    {
        return Compare( first, second ) > 0;
    }

    inline bool operator>=( StringView first, StringView second )
    {
        return Compare( first, second ) >= 0;
    }

    constexpr const char *begin( StringView view ) noexcept
    {
        return view.GetSubText();
    }

    constexpr const char *end( StringView view ) noexcept
    {
        return view.GetSubText() + view.GetCharCount();
    }
}

namespace std
{
    template <>
    struct formatter< smile::primitive::StringView >
    {
        formatter< basic_string_view< char > > Base;

        constexpr auto parse( format_parse_context &ctx )
        {
            return Base.parse( ctx );
        }

        auto format( const smile::primitive::StringView view, format_context &ctx ) const
        {
            basic_string_view< char > temp{ view.GetData(), static_cast< size_t >( view.GetCharCount() ) };
            return Base.format( temp, ctx );
        }
    };
}