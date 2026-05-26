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
 * @file        render_ids.h
 * @author      Zenn Geeraerts
 * @created     26 May 2026
 * @brief       Handle aliases for resources owned by the render world.
 *
 *              All render-world handles share the same underlying type
 *              (`primitive::Handle<Uint32, 24, 8>` — 24 index bits, 8 generation bits)
 *              so they slot directly into the existing `primitive::HandleManager`
 *              allocator without a wrapper layer. Type-level distinction between
 *              handle kinds is intentionally not enforced — the table itself is the
 *              authority on what a handle refers to.
 *
 *              `TextureBindlessIndex` is a different beast: a plain bindless
 *              descriptor-table slot with no generation, owned by the descriptor-heap
 *              allocator (not the render world).
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/handle.h"

namespace smile::graphic
{
    /** Underlying handle layout shared by every render-world resource. */
    using RenderHandle = primitive::Handle< Uint32, 24, 8 >;

    using RenderEntityId = RenderHandle;
    using MeshHandle = RenderHandle;
    using MaterialHandle = RenderHandle;
    using LightHandle = RenderHandle;
    using ViewHandle = RenderHandle;

    /** Bindless texture slot. No generation — recycled by the descriptor-heap allocator. */
    enum class TextureBindlessIndex : Uint32
    {
        Invalid = std::numeric_limits< Uint32 >::max(),
    };
}
