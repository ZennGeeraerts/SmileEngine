/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "string_view.h"

#include "smile/common/foundation/range_iterator.h"

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
}