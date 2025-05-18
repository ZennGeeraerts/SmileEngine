/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "assert.h"

#include "smile/common/logging/logging.h"

namespace smile::foundation
{
    bool
    HandleAssert( const char *condition, const char *message, const char *file, const int line, const char *function )
    {
        SM_LOG_ERROR( "Assertion '{0}' failed at {1}:{2} {3}", condition, file, line, message ? message : "" );
        return true;
    }
}