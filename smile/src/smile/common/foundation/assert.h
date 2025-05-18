/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiled.h"

namespace smile::foundation
{
    bool
    HandleAssert( const char *condition, const char *message, const char *file, const int line, const char *function );
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
#    define SM_ASSERT( condition, message )                                                                            \
        {                                                                                                              \
            if ( !( condition ) )                                                                                      \
            {                                                                                                          \
                ( smile::foundation::HandleAssert( #condition, message, __FILE__, __LINE__, __FUNCTION__ )             \
                        ? ( void )( SM_DEBUGBREAK() )                                                                  \
                        : ( void )0 );                                                                                 \
            }                                                                                                          \
        }
#else
#    define SM_ASSERT( x, ... )
#endif
