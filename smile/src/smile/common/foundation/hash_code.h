/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::foundation
{
    using HashCode = std::size_t;
    using HashCode32 = Uint32;

    inline constexpr foundation::HashCode HashCombine( foundation::HashCode seed, foundation::HashCode value )
    {
        return seed ^ ( value + 0x9e3779b97f4a7c15 + ( seed << 6 ) + ( seed >> 2 ) );
    }
}