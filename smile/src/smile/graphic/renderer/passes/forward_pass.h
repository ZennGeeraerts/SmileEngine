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
 *              frames (pipeline cache, constant buffers, binding objects).  Pass registration
 *              is done via free functions (AddForwardPass, AddSpritePass, AddUIPass) that
 *              call RenderGraph::AddPass() with inline lambdas — no virtual dispatch, no
 *              BeginPass/EndPass ceremony.
 */
#pragma once

#include "smile/common/primitive/collection/hash_map.h"
#include "smile/graphic/renderer/graphics_state.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/graphic/renderer/resource/graphics_pipeline.h"
#include "smile/graphic/renderer/shader/constant_buffer.h"
#include "smile/graphic/renderer/shader/binding_layout.h"
#include "smile/graphic/renderer/shader/binding_set.h"
#include "smile/graphic/renderer/render_graph/render_graph_resource.h"
#include "smile/graphic/renderer/view.h"

namespace smile::graphic
{
    class RenderContext;
    class ResourceManager;
    class MaterialSystem;
    class RenderScene;
    class RenderGraph;

    /**
     * All GPU state shared across the forward, sprite, and UI render passes.
     * One instance lives on the Renderer and is re-used every frame.
     */
    struct ForwardPassData final
    {
        ForwardPassData() = default;
        ~ForwardPassData() = default;

        ForwardPassData( const ForwardPassData & ) = delete;
        ForwardPassData &operator=( const ForwardPassData & ) = delete;

        void Initialize( RenderContext &context, ResourceManager &resourceManager, MaterialSystem &materialSystem );
        void ShutDown();

        // ---- Helpers called from pass execute lambdas (public to allow lambda capture by pointer) ----

        void SetupMaterial( MaterialInstance materialInstance, GraphicsState &graphicsState );

        primitive::HashMap< MaterialInstance, GraphicsPipeline >::Iterator CreatePipeline(
            MaterialInstance materialInstance );

        // ---- GPU objects (persistent across frames) ----

        primitive::HashMap< MaterialInstance, GraphicsPipeline > Pipelines;
        ConstantBuffer CameraCB;
        ConstantBuffer PerObjectCB;
        BindingLayout PassBindingLayout;
        BindingSet PassBindingSet;
        ViewConstants ViewCons{};

        // ---- Back-references set by Initialize() ----

        RenderContext *Context = nullptr;
        ResourceManager *ResourceMgr = nullptr;
        MaterialSystem *MaterialSys = nullptr;
    };

    /**
     * Registers a ForwardPass into the graph that renders all Renderables
     * in SceneLayer::World using the ForwardPassData pipeline.
     *
     * @param outColor  Output handle for SceneColor (created by this pass).
     * @param outDepth  Output handle for SceneDepth (created by this pass).
     */
    void AddForwardPass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        Uint32 width,
        Uint32 height,
        RenderGraphResourceHandle &outColor,
        RenderGraphResourceHandle &outDepth );

    /**
     * Registers a SpritePass that blends batched quads (SceneLayer::Sprite) on top of
     * the existing SceneColor/SceneDepth produced by AddForwardPass.
     */
    void AddSpritePass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        RenderGraphResourceHandle &inOutColor,
        RenderGraphResourceHandle &inOutDepth );

    /**
     * Registers a UIPass that renders screen-space overlays (SceneLayer::UI)
     * on top of the FinalColor buffer.
     */
    void AddUIPass( RenderGraph &graph,
        ForwardPassData &data,
        const RenderScene &scene,
        RenderGraphResourceHandle &inOutColor );
}
