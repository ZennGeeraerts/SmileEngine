/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "counted.h"

namespace smile::memory
{
    Counted::~Counted()
    {
        SM_ASSERT( m_RefCount == 0, "RefCount is not 0" );

        // Volatile ensures the compiler will write the value
        volatile auto &dest = m_RefCount;
        dest = DestructedObjectRefCount();
    }
}