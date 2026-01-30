/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/primitive/text/string_view.h"

#include <fmt/format.h>

namespace smile::logging
{
    using MemoryBuffer = fmt::basic_memory_buffer< char, 250 >;

    inline void AppendStringView( MemoryBuffer &dest, primitive::StringView view )
    {
        auto *pBuffer = view.GetSubText();
        dest.append( pBuffer, pBuffer + view.GetCharCount() );
    }
}