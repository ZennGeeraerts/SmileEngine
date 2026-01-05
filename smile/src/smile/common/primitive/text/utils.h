/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "string_view.h"

namespace smile::primitive
{
    template < typename Type >
    concept PureText = std::derived_from< Type, String >;

    template < typename Type >
    concept TextClass = PureText< Type > || std::same_as< Type, StringView >;

    template < TextClass Text >
    void Split( Vector< Text > &segments, const StringView text, const char seperator )
    {
        Index currentLineStartIndex{ 0 };
        Index currentCharIndex;

        for ( currentCharIndex = 0; currentCharIndex < text.GetCharCount(); ++currentCharIndex )
        {
            if ( text[currentCharIndex] == seperator )
            {
                if ( currentLineStartIndex != currentCharIndex )
                {
                    segments.PushBack(
                        { text.GetSubText() + currentLineStartIndex, currentCharIndex - currentLineStartIndex } );
                }

                currentLineStartIndex = currentCharIndex + 1;
            }
        }

        if ( currentLineStartIndex != text.GetCharCount() )
        {
            segments.PushBack(
                { text.GetSubText() + currentLineStartIndex, currentCharIndex - currentLineStartIndex } );
        }
    }

    std::optional< int > ToInt( StringView text );
    std::optional< float > ToFloat( StringView text );

    Index FindText( const primitive::StringView text,
        const primitive::StringView textToFind,
        const Index firstCharIndex = 0 );

    Index FindCharacter( const primitive::StringView text, const char charToFind, const Index firstCharIndex = 0 );

    void ReplaceText( String &text, const StringView textToFind, const StringView textToReplace );

    void
    ReplaceTextInsideRange( String &text, const Index index, const Count charCount, const StringView textToReplace );

    void ToLowerCase( String &text );
    void ToUpperCase( String &text );
}
