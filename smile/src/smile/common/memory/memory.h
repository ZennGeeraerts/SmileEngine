/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

namespace smile::memory
{
    constexpr Uint32 g_AlignedSize = 8; // x64
    constexpr Uint32 g_DefaultAllocatorAlignedSize = 8;

    constexpr Uint32 GetAlignedSize( const Uint32 size )
    {
        return ( size + g_AlignedSize - 1 ) & ~( g_AlignedSize - 1 );
    }

    Uint32 GetAllocatedSize( const Uint32 size, const bool addHeaderSize = true );
}