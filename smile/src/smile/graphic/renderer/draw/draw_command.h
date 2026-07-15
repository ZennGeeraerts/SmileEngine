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
 * @file        draw_command.h
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       CPU-sided submission unit for a single indexed draw call.
 *
 *              DrawCommand holds only CPU-side descriptors. The render-graph pass execute
 *              lambda is responsible for resolving them to GPU commands:
 *
 *                Mesh            -> VertexBuffer/IndexBuffer bindings + index count
 *                MaterialInstance -> PSO (pipeline) + shader binding set
 *                WorldTransform  -> per-object constant buffer upload
 *
 *              This separation keeps scene extraction free of GPU dependencies and
 *              ensures the SortKey is computed before any GPU state is touched.
 *              InstanceCount > 1 triggers DrawIndexedInstanced; the pass builds the
 *              per-instance data from a separate instancing buffer.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/renderer/draw/sort_key.h"
#include "smile/graphic/renderer/mesh/mesh.h"
#include "smile/graphic/renderer/material/material.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    struct DrawCommand final
    {
        SortKey Key;

        // CPU mesh descriptor — holds RHI buffer handles and topology.
        // The pass resolves these to VB/IB bindings and derives the index count
        // from Mesh.IndexBuffer.GetIndexCount().
        Mesh Mesh;

        // CPU material handle — the pass resolves this to a PSO and binding set.
        MaterialInstance Material;

        // Per-instance world transform uploaded to the per-object constant buffer.
        DirectX::XMFLOAT4X4 WorldTransform{};

        // Instancing: 1 = non-instanced draw (DrawIndexed).
        // Values > 1 = DrawIndexedInstanced; the pass sources per-instance data
        // from a separate buffer assembled during scene extraction.
        Count InstanceCount = 1;
        Count StartInstance = 0;
    };
}
