/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "object.h"

namespace smile::memory
{
    Object::~Object()
    {
        SM_ASSERT( m_RefCount == 0, "RefCount is not 0" );

        volatile auto &dest = m_RefCount;
        dest = DestructedObjectRefCount();
    }

    constexpr Uint32 Object::DestructedObjectRefCount()
    {
        return std::numeric_limits< Uint32 >{}.max();
    }
}