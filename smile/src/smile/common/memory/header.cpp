/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "header.h"

namespace smile::memory
{
    Uint32 Header::s_NextAllocatorIndex = 0;
}

static_assert( sizeof( smile::memory::Header ) == smile::memory::g_DefaultAllocatorAlignedSize );