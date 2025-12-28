/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        enum_concepts.h
 * @author      Zenn Geeraerts
 * @created     28 December 2025
 * @brief       Concepts to be used with enums
 */
#pragma once

#include <type_traits>

namespace smile::foundation
{
    template < typename Type >
    concept EnumWithCount = std::is_enum_v< Type > && requires { Type::Count; };
}