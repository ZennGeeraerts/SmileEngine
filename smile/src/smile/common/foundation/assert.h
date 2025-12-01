/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiled.h"

namespace smile::foundation
{
    using HandleAssertFunction = bool ( * )( const char *condition,
        const char *message,
        const char *file,
        const int line,
        const char *function );

    bool
    HandleAssert( const char *condition, const char *message, const char *file, const int line, const char *function );

    void SetHandleAssertFunction( HandleAssertFunction func );

    bool DefaultHandleAssert( const char *condition,
        const char *message,
        const char *file,
        const int line,
        const char *function );

    bool
    ThrowOnAssert( const char *condition, const char *message, const char *file, const int line, const char *function );
}

#ifdef SM_C_DEBUG
#    ifdef SM_PLATFORM_WINDOWS
#        define SM_DEBUGBREAK() __debugbreak()
#    else
#        error "Platform doesn't support debugbreak"
#    endif
#    define SM_ENABLE_ASSERTS
#endif

#ifdef SM_ENABLE_ASSERTS
#    define SM_ASSERT( condition )                                                                                     \
        {                                                                                                              \
            if ( !( condition ) )                                                                                      \
            {                                                                                                          \
                ( smile::foundation::HandleAssert( #condition, nullptr, __FILE__, __LINE__, __FUNCTION__ )             \
                        ? ( void )( SM_DEBUGBREAK() )                                                                  \
                        : ( void )0 );                                                                                 \
            }                                                                                                          \
        }
#    define SM_ASSERT_MSG( condition, message )                                                                        \
        {                                                                                                              \
            if ( !( condition ) )                                                                                      \
            {                                                                                                          \
                ( smile::foundation::HandleAssert( #condition, message, __FILE__, __LINE__, __FUNCTION__ )             \
                        ? ( void )( SM_DEBUGBREAK() )                                                                  \
                        : ( void )0 );                                                                                 \
            }                                                                                                          \
        }
#else
#    define SM_ASSERT( x )
#    define SM_ASSERT_MSG( x, ... )
#endif

#if SM_OPTION_UNIT_TESTS
#    define REQUIRE_ASSERT( expression )                                                                               \
        {                                                                                                              \
            smile::foundation::SetHandleAssertFunction( smile::foundation::ThrowOnAssert );                            \
            REQUIRE_THROWS( expression );                                                                              \
        }

#    define CHECK_ASSERT( expression )                                                                                 \
        {                                                                                                              \
            smile::foundation::SetHandleAssertFunction( smile::foundation::ThrowOnAssert );                            \
            CHECK_THROWS( expression );                                                                                \
        }

#    define REQUIRE_NOASSERT( expression )                                                                             \
        {                                                                                                              \
            smile::foundation::SetHandleAssertFunction( smile::foundation::ThrowOnAssert );                            \
            REQUIRE_NOTHROW( expression );                                                                             \
        }

#    define CHECK_NOASSERT( expression )                                                                               \
        {                                                                                                              \
            smile::foundation::SetHandleAssertFunction( smile::foundation::ThrowOnAssert );                            \
            CHECK_NOTHROW( expression );                                                                               \
        }
#endif
