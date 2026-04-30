/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        material_handle.h
 * @author      Zenn Geeraerts
 * @created     30 April 2026
 * @brief       Defines the material handle types
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/handle_manager.h"

namespace smile::graphic::detail
{
    using MaterialHandleManager = primitive::HandleManager< Uint32, 24, 8 >;
    using MaterialHandle = MaterialHandleManager::HandleType;

    using MaterialInstanceHandleManager = primitive::HandleManager< Uint32, 24, 8 >;
    using MaterialInstanceHandle = MaterialInstanceHandleManager::HandleType;
}