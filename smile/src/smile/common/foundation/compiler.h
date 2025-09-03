/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#if defined _MSC_VER
#    define SM_COMPILER_MSVC
#elif defined __clang__
#    define SM_COMPILER_CLANG
#elif defined __GNU__
#    define SM_COMPILER_GNU
#elif defined __GNUC__
#    define SM_COMPILER_GNUC
#else
#    error Unsupported compiler
#endif