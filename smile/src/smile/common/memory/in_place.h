/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "smile/common/foundation/compiled.h"

namespace smile::memory
{
    class InPlace;

    constexpr InPlace *g_pInPlace = nullptr;
}

inline void *operator new( std::size_t, void *pAllocatedByteArray, const smile::memory::InPlace * ) noexcept
{
    return pAllocatedByteArray;
}