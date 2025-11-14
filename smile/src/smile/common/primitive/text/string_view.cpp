/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "string_view.h"

#include "smile/common/foundation/range_iterator.h"
#include "utils.h"

namespace smile::primitive
{
    int Compare( StringView firstText, StringView secondText )
    {
        Count charCount = std::min( firstText.GetCharCount(), secondText.GetCharCount() );

        for ( Index index : foundation::GetCountIterator( charCount ) )
        {
            if ( firstText[index] != secondText[index] )
                return firstText[index] - secondText[index];
        }

        return firstText.GetCharCount() - secondText.GetCharCount();
    }

    Index StringView::Find( const char charToFind, const Index firstCharIndex ) const
    {
        return FindCharacter( *this, charToFind, firstCharIndex );
    }

    Index StringView::Find( const StringView textToFind, const Index firstCharIndex ) const
    {
        return FindText( *this, textToFind, firstCharIndex );
    }
}