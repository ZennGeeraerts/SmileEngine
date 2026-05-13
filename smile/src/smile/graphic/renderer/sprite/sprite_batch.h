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
 * @file        sprite_batch.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       CPU-side sprite batching — groups sprites by material, outputs Renderables per frame
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/hash_map.h"
#include "smile/graphic/renderer/material/material.h"
#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class RenderScene;
    class ResourceManager;
    class RenderContext;
    class ShaderLibrary;

    /**
     * One quad pushed to a sprite batch this frame.
     */
    struct SpriteSubmitInfo final
    {
        DirectX::XMFLOAT4X4 WorldTransform;
        MaterialInstance Material;
    };

    /**
     * Batches sprites by material each frame and emits one Renderable per batch
     * into the RenderScene::Sprite layer.
     *
     * Lifecycle per frame:
     *   1. Submit() — called for every visible sprite (e.g. from SceneExtractor)
     *   2. Flush()  — called by FrameGraph; uploads CPU data to dynamic VBs and
     *                 writes one Renderable per batch into the scene
     *   3. Reset()  — called by FrameGraph after the frame; clears CPU buckets
     */
    class SpriteBatch final
    {
      public:
        /**
         * Maximum number of quads in a single dynamic vertex buffer.
         * Larger scenes use multiple batches per material.
         */
        static constexpr Count s_MaxQuadsPerBatch = 2048;
        static constexpr Count s_VerticesPerQuad = 4;
        static constexpr Count s_IndicesPerQuad = 6;

        SpriteBatch() = default;
        ~SpriteBatch() = default;

        SpriteBatch( const SpriteBatch & ) = delete;
        SpriteBatch &operator=( const SpriteBatch & ) = delete;

        /**
         * Initialises shared GPU resources (shared index buffer, vertex layout).
         * Must be called once before the first frame.
         */
        void Initialize( ResourceManager &resourceManager );

        /**
         * Destroys all GPU resources. Must be called before the ResourceManager is torn down.
         */
        void ShutDown( ResourceManager &resourceManager );

        /**
         * Enqueues a sprite for this frame.
         * Thread-safe relative to other Submit() calls only if called from the same thread.
         */
        void Submit( const DirectX::XMFLOAT4X4 &worldTransform, MaterialInstance material );

        void Submit( const DirectX::XMFLOAT2 &position,
            float rotation,
            const DirectX::XMFLOAT2 &size,
            MaterialInstance material );

        /**
         * Uploads all pending sprite data to GPU, then writes one Renderable per batch
         * into scene's Sprite layer.  Called once per frame by FrameGraph before Compile().
         */
        void Flush( RenderScene &scene, ResourceManager &resourceManager, RenderContext &context );

        /**
         * Clears the per-frame CPU buckets.  Called by FrameGraph after the frame ends.
         */
        void Reset();

      private:
        struct SpriteVertex final
        {
            DirectX::XMFLOAT3 Position;
            DirectX::XMFLOAT2 TexCoord;
        };

        struct Bucket final
        {
            primitive::Vector< SpriteVertex > Vertices;
            VertexBuffer DynamicVB;
        };

        [[nodiscard]] VertexBuffer &
        GetOrCreateDynamicVB( Bucket &bucket, Count vertexCount, ResourceManager &resourceManager );

      private:
        primitive::HashMap< MaterialInstance, Bucket > m_Buckets;

        IndexBuffer m_SharedIndexBuffer;
        rhi::BufferLayout m_VertexLayout;

        bool m_IsInitialized = false;
    };
}
