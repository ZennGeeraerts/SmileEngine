/*=============================================================================*/
// Copyright 2022-2024 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "compiled.h"

namespace smile::foundation
{
    class ConstantText final
    {
      public:
        constexpr ConstantText() = default;
        constexpr ConstantText( const ConstantText & ) = default;
        constexpr ConstantText( const char *text, const Count charCount ) : m_Text{ text }, m_CharCount{ charCount }
        {
        }

        template < int CharCount >
        constexpr ConstantText( const char ( &text )[CharCount] ) : m_Text{ text }, m_CharCount{ CharCount - 1 }
        {
        }

        constexpr ConstantText Skip( const Count charCount ) const
        {
            return ConstantText{ m_Text + charCount, m_CharCount - charCount };
        }

        constexpr ConstantText Shrink( const Count charCount ) const
        {
            return ConstantText{ m_Text, m_CharCount - charCount };
        }

        constexpr ConstantText SubStr( const Index pos, const Count charCount )
        {
            SM_ASSERT( m_CharCount >= pos + charCount );

            return ConstantText{ m_Text + pos, charCount };
        }

        constexpr const char *GetText() const
        {
            return m_Text;
        }

        constexpr Count GetCharCount() const
        {
            return m_CharCount;
        }

        constexpr bool IsEmpty() const
        {
            return m_CharCount == 0;
        }

        template < Count CharCount >
        constexpr bool StartsWith( const char ( &startingText )[CharCount] ) const
        {
            return StartsWith( startingText, CharCount - 1 );
        }

        constexpr bool StartsWith( const char *startingText, const Count charCount ) const
        {
            return m_CharCount >= 0 &&
                   ( ( charCount == 0 ) ||
                       ( *m_Text == *startingText && Skip( 1 ).StartsWith( startingText + 1, charCount - 1 ) ) );
        }

        template < Count CharCount >
        constexpr Index Find( const char ( &text )[CharCount] ) const
        {
            return Find( text, CharCount - 1 );
        }

        constexpr Index Find( const char *text, const Count charCount ) const
        {
            for ( Index i{}; i < m_CharCount; ++i )
            {
                if ( Skip( i ).StartsWith( text, charCount ) )
                {
                    return i;
                }
            }

            return s_InvalidIndex;
        }

        constexpr Index ReverseFind( const char element ) const
        {
            for ( Index i{ m_CharCount }; i > 0; --i )
            {
                if ( element == m_Text[i - 1] )
                    return i - 1;
            }

            return s_InvalidIndex;
        }

        char operator[]( const Index index ) const
        {
            return m_Text[index];
        }

        constexpr bool operator==( const ConstantText &other ) const
        {
            if ( m_CharCount != other.m_CharCount )
                return false;

            for ( Index i{}; i < m_CharCount; ++i )
            {
                if ( m_Text[i] != other.m_Text[i] )
                    return false;
            }

            return true;
        }

        template < int CharCount >
        constexpr bool operator==( const char ( &other )[CharCount] ) const
        {
            if ( ( CharCount - 1 ) != m_CharCount )
                return false;

            for ( Index i{}; i < m_CharCount; ++i )
            {
                if ( m_Text[i] != other[i] )
                    return false;
            }

            return true;
        }

        constexpr ConstantText &operator=( const ConstantText & ) = default;

      private:
        const char *m_Text{ nullptr };
        Count m_CharCount = 0;
    };
}