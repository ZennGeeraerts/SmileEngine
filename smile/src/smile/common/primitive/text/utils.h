/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "string_view.h"

namespace smile::primitive
{
    std::optional< int > ToInt( StringView text );
    std::optional< float > ToFloat( StringView text );

    Index FindText( const primitive::StringView text,
        const primitive::StringView textToFind,
        const Index firstCharIndex = 0 );

    Index FindCharacter( const primitive::StringView text, const char charToFind, const Index firstCharIndex = 0 );

    void ReplaceText( String &text, const StringView textToFind, const StringView textToReplace );

    void
    ReplaceTextInsideRange( String &text, const Index index, const Count charCount, const StringView textToReplace );
}
