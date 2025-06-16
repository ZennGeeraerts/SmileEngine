/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "assert.h"

#include "smile/common/logging/logging.h"
#include "assert_failed_exception.h"

namespace smile::foundation
{
    bool ( *g_HandleAssertFunction )( const char *condition,
        const char *message,
        const char *file,
        const int line,
        const char *function ) = &DefaultHandleAssert;

    bool
    HandleAssert( const char *condition, const char *message, const char *file, const int line, const char *function )
    {
        return g_HandleAssertFunction( condition, message, file, line, function );
    }

    void SetHandleAssertFunction( HandleAssertFunction func )
    {
        g_HandleAssertFunction = func;
    }

    bool DefaultHandleAssert( const char *condition,
        const char *message,
        const char *file,
        const int line,
        const char *function )
    {
        SM_LOG_ERROR( "Assertion '{0}' failed at {1}:{2} {3}", condition, file, line, message ? message : "" );
        return true;
    }

    bool
    ThrowOnAssert( const char *condition, const char *message, const char *file, const int line, const char *function )
    {
        throw AssertFailedException{ condition };
        return false;
    }
}