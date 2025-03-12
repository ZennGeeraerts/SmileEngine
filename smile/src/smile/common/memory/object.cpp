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

        // Volatile ensures the compiler will write the value
        volatile auto &dest = m_RefCount;
        dest = DestructedObjectRefCount();
    }
}