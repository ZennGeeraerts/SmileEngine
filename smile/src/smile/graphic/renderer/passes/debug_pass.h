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
 * @file        debug_pass.h
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       DebugPassData and AddDebugPass builder function.
 *
 *              DebugPassData owns the GPU pipeline and vertex buffer for line rendering.
 *              Game / editor code submits lines directly via DrawLine() before the graph
 *              is built.  AddDebugPass() registers the execute lambda that uploads the
 *              CPU line list and issues the draw call.  Reset() discards per-frame lines.
 */
#pragma once

#include "smile/common/primitive/collection/vector.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/render_graph/render_graph_resource.h"
#include "smile/graphic/rhi/resource/buffer.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class RenderContext;
    class ResourceManager;
    class ShaderLibrary;
    class View;
    class RenderGraph;

    /**
     * Persistent GPU state for debug line rendering.
     * Owns the pipeline, dynamic VB, camera CB, and per-frame line list.
     */
    struct DebugPassData final
    {
        DebugPassData() = default;
        ~DebugPassData() = default;

        DebugPassData( const DebugPassData & ) = delete;
        DebugPassData &operator=( const DebugPassData & ) = delete;

        void Initialize( RenderContext &context, ResourceManager &resourceManager, const ShaderLibrary &shaderLib );
        void ShutDown();

        /** Clears per-frame transient lines. Call after each frame. */
        void Reset();

        // ---- Line submission (called before Build()) ----

        void DrawLine( const DirectX::XMFLOAT3 &start, const DirectX::XMFLOAT3 &end, const DirectX::XMFLOAT4 &color );

        void DrawLine( const DirectX::XMFLOAT3 &start,
            const DirectX::XMFLOAT3 &end,
            const DirectX::XMFLOAT4 &colorStart,
            const DirectX::XMFLOAT4 &colorEnd );

        // ---- Internal vertex type ----

        struct VertexPosCol final
        {
            DirectX::XMFLOAT3 Position;
            DirectX::XMFLOAT4 Color;
        };

        // ---- GPU objects (persistent) ----

        GraphicsPipeline Pipeline;
        VertexBuffer DynamicVB;
        ConstantBuffer CameraCB;
        BindingSet PassBindingSet;
        Count VertexCapacity{ 100 };
        const rhi::BufferLayout m_VertexLayout{ { rhi::Format::RGB32_FLOAT, "POSITION" },
            { rhi::Format::RGBA32_FLOAT, "COLOR" } };

        // ---- Per-frame CPU data ----

        primitive::Vector< VertexPosCol > LineList;

        // ---- Back-references ----

        RenderContext *Context = nullptr;
        ResourceManager *ResourceMgr = nullptr;

        void GrowVertexBuffer();
        void CreateFixedLineList();
    };

    /**
     * Registers a DebugPass into the graph that renders all lines submitted to @p data
     * plus the fixed grid/axis overlay.
     *
     * Reads and overwrites @p inOutColor (does not write depth).
     */
    void
    AddDebugPass( RenderGraph &graph, DebugPassData &data, const View &view, RenderGraphResourceHandle &inOutColor );
}
