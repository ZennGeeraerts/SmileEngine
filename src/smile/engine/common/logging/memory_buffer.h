/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <fmt/format.h>

namespace smile::logging
{
    using MemoryBuffer = fmt::basic_memory_buffer< char, 250 >;

    inline void AppendStringView( MemoryBuffer &dest, std::string_view view )
    {
        auto *pBuffer = view.data();
        dest.append( pBuffer, pBuffer + view.size() );
    }
}