/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/constant_text.h"
#include "smile/common/foundation/numeric_cast.h"
#include "smile/common/memory/memory.h"

namespace smile::primitive
{
    class StringView final
    {
      public:
        StringView() = default;

        StringView( foundation::ConstantText text ) : m_SubText{ text.GetText() }, m_CharCount{ text.GetCharCount() }
        {
        }

        StringView( const char *text )
            : m_SubText{ text }, m_CharCount{ text ? foundation::NumericCast< Count >( std::strlen( text ) ) : 0 }
        {
        }

        template < int Size >
        StringView( const char ( &text )[Size] ) : StringView{ text, Size - 1 }
        {
        }

        StringView( const char *text, const Count charCount ) : m_SubText{ text }, m_CharCount{ charCount }
        {
        }

        ~StringView() = default;

        inline char operator[]( const Index index ) const
        {
            SM_ASSERT( index < m_CharCount );

            return m_SubText[index];
        }

        Count GetCharCount() const
        {
            return m_CharCount;
        }

        const char *GetData() const
        {
            SM_ASSERT( IsNullTerminated() );

            return m_SubText;
        }

        const char *GetSubText() const
        {
            return m_SubText;
        }

        inline bool IsEmpty() const
        {
            return m_CharCount == 0;
        }

        bool IsNullTerminated() const
        {
            return m_SubText[m_CharCount] == 0;
        }

        bool IsValidRange( const Index firstIndex, const Count charCount ) const
        {
            return ( firstIndex + charCount ) <= m_CharCount;
        }

        inline const char *begin()
        {
            return m_SubText;
        }

        inline const char *end()
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

    inline const char *begin( StringView view )
    {
        return view.GetSubText();
    }

    inline const char *end( StringView view )
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