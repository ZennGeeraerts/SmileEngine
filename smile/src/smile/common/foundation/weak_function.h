/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/platform/platform_detection.h"

#if SM_PLATFORM_WINDOWS
#    define SM_FOUNDATION_WEAK_FUNCTION_DECLARE( function ) extern "C" __declspec( dllexport ) function;

#    define SM_FOUNDATION_WEAK_FUNCTION_RESOLVE( functionPointerType, functionPointer, function )                      \
        functionPointer = functionPointerType( GetProcAddress( GetModuleHandle( NULL ), #function ) );                 \
                                                                                                                       \
        if ( !functionPointer )                                                                                        \
        {                                                                                                              \
            functionPointer = &function##Default;                                                                      \
        }

#    define SM_FOUNDATION_WEAK_FUNCTION_IMPLEMENT( returnType, function, ... )                                         \
        __pragma( comment( linker,                                                                                     \
            "/alternatename:"                                                                                          \
            "_" #function "=_"                                                                                         \
            "Default" #function ) );                                                                                   \
        extern "C" returnType function##Default( __VA_ARGS__ )

#else
#    define SM_FOUNDATION_WEAK_FUNCTION_DECLARE( function ) function __attribute__( ( weak ) );

#    define SM_FOUNDATION_WEAK_FUNCTION_RESOLVE( functionPointerType, functionPointer, function )                      \
        functionPointer = &function;

#    define SM_FOUNDATION_WEAK_FUNCTION_IMPLEMENT( returnType, function, ... )                                         \
        returnType function( __VA_ARGS__ ) __attribute__( ( weak ) );                                                  \
                                                                                                                       \
        returnType function( __VA_ARGS__ )
#endif