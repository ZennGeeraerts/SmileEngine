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
 * @file        forward_pass.h
 * @author      Zenn Geeraerts
 * @created     13 May 2026
 * @brief       ForwardPassData and pass-builder free functions for forward-lit geometry.
 *
 *              ForwardPassData is a plain struct that owns GPU resources persisted across
 *              frames (constant buffers, binding objects).  The pipeline cache lives in
 *              RenderWorld and is populated during the Prepare step.
 *              Pass registration is done via the AddForwardPass free function that
 *              calls RenderGraph::AddPass() with an inline lambda — no virtual dispatch, no
 *              BeginPass/EndPass ceremony.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/graphic/renderer/render_graph/render_graph_resource.h"

namespace smile::graphic
{
    class RenderContext;
    class ResourceManager;
    class MaterialSystem;
    class RenderWorld;
    class DrawCommandBuffer;
    class RenderGraph;

    /**
     * Thin pass-data struct — owns only the three back-references set by Initialize().
     * All GPU resources (constant buffers, binding sets, pipelines) live in RenderWorld
     * and are created or cached during the Prepare step.
     */
    struct ForwardPassData final
    {
        ForwardPassData() = default;
        ~ForwardPassData() = default;

        ForwardPassData( const ForwardPassData & ) = delete;
        ForwardPassData &operator=( const ForwardPassData & ) = delete;

        void Initialize( RenderContext &context, ResourceManager &resourceManager, MaterialSystem &materialSystem );
        void ShutDown();

        // ---- Back-references set by Initialize() ----

        RenderContext *Context = nullptr;
        ResourceManager *ResourceMgr = nullptr;
        MaterialSystem *MaterialSys = nullptr;
    };

    /**
     * Registers a ForwardPass into the graph that renders all opaque geometry
     * from @p buffer using the pipeline components stored on entities in @p renderWorld.
     * The camera constant buffer must already be filled (by RenderWorld::Prepare) before
     * the render graph executes.
     *
     * @param outColor  Output handle for SceneColor (created by this pass).
     * @param outDepth  Output handle for SceneDepth (created by this pass).
     */
    void AddForwardPass( RenderGraph &graph,
        ForwardPassData &data,
        RenderWorld &renderWorld,
        const DrawCommandBuffer &buffer,
        Uint32 width,
        Uint32 height,
        RenderGraphResourceHandle &outColor,
        RenderGraphResourceHandle &outDepth );
}
