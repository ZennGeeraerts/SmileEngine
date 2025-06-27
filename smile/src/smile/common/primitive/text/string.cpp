/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "string.h"

namespace smile::primitive
{
    String::String( const char *text ) : String{ StringView{ text } }
    {
    }

    String &String::operator+=( StringView view )
    {
        Count charCount = GetCharCount();
        Count otherCharCount = view.GetCharCount();

        SetCharCount( charCount + otherCharCount );

        if ( GetCharCount() && otherCharCount )
        {
            std::memcpy( &( *this )[charCount], view.GetSubText(), otherCharCount );
        }

        return *this;
    }

    String &String::operator+=( const char item )
    {
        Count charCount = GetCharCount();
        SetCharCount( charCount + 1 );

        ( *this )[charCount] = item;

        return *this;
    }

    void String::Set( StringView view )
    {
        Count distance = view.GetSubText() - GetData();

        if ( distance >= 0 && distance < GetCharCount() )
        {
            String copy{ view.GetSubText(), view.GetCharCount() };
            *this = std::move( copy );
        }
        else
        {
            Set( view.GetSubText(), view.GetCharCount() );
        }
    }

    void String::Set( const char *text, const Count charCount )
    {
        SetCharCount( charCount );
        std::memcpy( GetData(), text, charCount );
    }

    void String::EraseCharAtIndex( const Index index )
    {
        SM_ASSERT( index < GetCharCount() );

        EraseAtIndex( index );
    }

    String operator+( StringView leftView, StringView rightView )
    {
        String result;
        result.Reserve( leftView.GetCharCount() + rightView.GetCharCount() );
        result = leftView;
        result += rightView;

        return result;
    }
}