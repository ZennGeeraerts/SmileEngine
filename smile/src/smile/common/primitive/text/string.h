/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/constant_text.h"
#include "smile/common/foundation/identifier.h"
#include "smile/common/primitive/collection/vector.h"
#include "string_view.h"

namespace smile::primitive
{
    class String final : protected Vector< char >
    {
      public:
        String() = default;
        String( const String & ) = default;

        String( StringView view )
        {
            Set( view );
        }

        String( const foundation::ConstantText &text )
        {
            Set( text.GetText(), text.GetCharCount() );
        }

        template < int Size >
        String( const char ( &text )[Size] ) : String{ text, Size - 1 }
        {
            SM_ASSERT_MSG( Size - 1 == StringView{ text }.GetCharCount(),
                "Array should be a char * if it contains a null character" );
        }

        String( const char *text );

        String( const char *text, const Count charCount )
        {
            Set( text, charCount );
        }

        String( String &&text ) noexcept : Vector< char >{ std::move( text ) }
        {
        }

        ~String() = default;

        String &operator=( const String & ) = default;

        using Vector< char >::operator[];

        operator StringView() const
        {
            return { GetData(), GetCharCount() };
        }

        bool IsValidRange( const Index firstIndex, const Count count ) const
        {
            return firstIndex + count <= GetCharCount();
        }

        char *GetData()
        {
            static char emptyStr[] = "";

            return GetItemCount() > 0 ? Vector< char >::GetData() : emptyStr;
        }

        const char *GetData() const
        {
            return GetItemCount() > 0 ? Vector< char >::GetData() : "";
        }

        bool IsEmpty() const
        {
            return GetCharCount() == 0;
        }

        Count GetCharCount() const
        {
            Count count = GetItemCount();
            return count > 0 ? count - 1 : 0;
        }

        char GetLastChar() const
        {
            return GetItemAtIndex( GetCharCount() - 1 );
        }

        void Set( StringView view );
        void Set( const char *text, const Count charCount );

        void SetCharCount( const Count count )
        {
            if ( count > 0 )
            {
                Vector< char >::SetItemCount( count + 1 );
                Vector< char >::operator[]( count ) = 0;
            }
            else
            {
                Clear();
            }
        }

        void Reserve( const Count count )
        {
            Vector< char >::Reserve( count + 1 );
        }

        using Vector< char >::Clear;

        void EraseCharAtIndex( const Index index );

        StringView AsStringView( const Index firstCharIndex = 0 ) const
        {
            return { GetData() + firstCharIndex, GetCharCount() - firstCharIndex };
        }

        String &operator=( String &&other ) noexcept
        {
            Vector< char >::operator=( std::move( other ) );
            return *this;
        }

        template < typename OtherType >
        String &operator=( StringView other )
        {
            Set( other );
            return *this;
        }

        String &operator+=( StringView view );

        String &operator+=( const char item );
    };

    String operator+( StringView leftView, StringView rightView );
}

namespace std
{
    template <>
    struct hash< smile::primitive::String >
    {
        smile::foundation::HashCode operator()( const smile::primitive::String &text ) const
        {
            return smile::foundation::identifier::GetHashCode( text.GetData() );
        }
    };
}