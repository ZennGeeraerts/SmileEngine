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
 * @file        debug_renderer.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Transient debug primitive renderer — lines, gizmos, and wireframe overlays
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/renderer/passes/debug_pass.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class ResourceManager;
    class ShaderLibrary;

    /**
     * Immediate-mode debug rendering for transient primitives.
     * All drawn geometry is discarded at frame end — nothing persists.
     *
     * Lifecycle per frame:
     *   1. DrawLine() / DrawAABB() / DrawSphere() — called from gameplay / editor code
     *   2. Flush(data) — appends CPU vertices into DebugPassData::LineList
     *   3. Reset() — clears the CPU buffer after the frame
     *
     * GPU resources (pipeline, VB, CB) live in DebugPassData, not here.
     */
    class DebugRenderer final
    {
      public:
        DebugRenderer() = default;
        ~DebugRenderer() = default;

        DebugRenderer( const DebugRenderer & ) = delete;
        DebugRenderer &operator=( const DebugRenderer & ) = delete;

        // ---- Draw commands (valid between Reset() and Flush()) ------

        void DrawLine( const DirectX::XMFLOAT3 &start, const DirectX::XMFLOAT3 &end, const DirectX::XMFLOAT4 &color );

        void DrawLine( const DirectX::XMFLOAT3 &start,
            const DirectX::XMFLOAT3 &end,
            const DirectX::XMFLOAT4 &colorStart,
            const DirectX::XMFLOAT4 &colorEnd );

        void DrawAABB( const DirectX::XMFLOAT3 &min,
            const DirectX::XMFLOAT3 &max,
            const DirectX::XMFLOAT4 &color = { 0.0f, 1.0f, 0.0f, 1.0f } );

        void DrawSphere( const DirectX::XMFLOAT3 &center,
            float radius,
            const DirectX::XMFLOAT4 &color = { 0.0f, 1.0f, 1.0f, 1.0f },
            Uint32 segments = 16 );

        // ---- Frame lifecycle ----------------------------------------

        /**
         * Appends all pending CPU vertices into @p data.LineList so that AddDebugPass
         * can upload and draw them. Call once per frame before Build().
         */
        void Flush( DebugPassData &data );

        /**
         * Clears the per-frame CPU buffer. Call once per frame after EndFrame().
         */
        void Reset();

      private:
        struct DebugVertex final
        {
            DirectX::XMFLOAT3 Position;
            DirectX::XMFLOAT4 Color;
        };

      private:
        primitive::Vector< DebugVertex > m_Vertices;
    };
}
