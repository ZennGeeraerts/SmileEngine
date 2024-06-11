/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "compiled.h"
#include "smile_engine/common/logger/logger.h"

#ifdef SM_ENABLE_ASSERTS
#    define SM_ASSERT( x, ... )                                                                                        \
        {                                                                                                              \
            if ( !( x ) )                                                                                              \
            {                                                                                                          \
                SM_LOG_ERROR( "Assertion Failed: %s", __VA_ARGS__ );                                                   \
                SM_DEBUGBREAK();                                                                                       \
            }                                                                                                          \
        }
#else
#    define SM_ASSERT( x, ... )
#endif
