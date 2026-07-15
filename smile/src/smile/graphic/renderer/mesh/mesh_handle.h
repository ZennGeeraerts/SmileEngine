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
 * @file        mesh_handle.h
 * @author      Zenn Geeraerts
 * @created     5 June 2026
 * @brief       Defines handles for GPU mesh resources.
 */
#pragma once

#include "smile/common/primitive/handle_manager.h"

namespace smile::graphic
{
    using MeshHandleManager = primitive::HandleManager< Uint64, 32u, 32u, struct Mesh >;
    using MeshHandle = MeshHandleManager::HandleType;
}