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

        inline bool operator==( StringView other ) const
        {
            return m_CharCount == other.m_CharCount &&
                   memory::CompareByteArrays( m_SubText, other.m_SubText, other.m_CharCount );
        }

        inline bool operator!=( StringView other ) const
        {
            return !( *this == other );
        }

        inline const char *begin()
        {
            return m_SubText;
        }

        inline const char *end()
        {
            return m_SubText + m_CharCount;
        }

      private:
        const char *m_SubText{ nullptr };
        Count m_CharCount{ 0 };
    };

    int Compare( StringView firstText, StringView secondText );

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