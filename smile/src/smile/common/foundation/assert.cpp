/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "assert.h"

#include "logging/logging.h"

namespace smile::foundation
{
    void
    HandleAssert( const char *condition, const char *message, const char *file, const int line, const char *function )
    {
        SM_LOG_ERROR( "Assertion Failed: {}", message );
        SM_DEBUGBREAK();  
    }
}