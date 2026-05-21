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
 * @file        render_scene.h
 * @author      Zenn Geeraerts
 * @created     12 May 2026
 * @brief       Per-frame snapshot of visible scene objects, separated by rendering layer
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/enum_array.h"
#include "renderable.h"
#include "view.h"
#include "resource/frame_buffer.h"

namespace smile::graphic
{
    /**
     * Identifies which rendering pass is responsible for a renderable.
     * Values must remain ordered from world-back to screen-front — they map to
     * execution order inside FrameGraph.
     */
    enum class SceneLayer : Uint8
    {
        World,  ///< Opaque 3D meshes — consumed by the ForwardPass
        Sprite, ///< 2D sprites — batched by SpriteBatch, consumed by the SpritePass
        Debug,  ///< Transient debug primitives (lines, gizmos) — consumed by the DebugPass
        UI,     ///< Screen-space overlays — consumed by the UIPass (always on top)

        Count
    };

    /**
     * Per-frame snapshot of all renderable objects, organised by SceneLayer.
     *
     * Each layer maps to one or more render graph passes:
     *  - SceneLayer::World  → ForwardPass  (opaque + transparent 3D)
     *  - SceneLayer::Sprite → SpritePass   (2D sprite batches)
     *  - SceneLayer::Debug  → DebugPass    (transient lines / gizmos)
     *  - SceneLayer::UI     → UIPass       (screen-space overlays)
     *
     * The scene is populated during scene extraction (ECS → RenderScene) at the
     * start of each frame and cleared at the end by the FrameGraph.
     */
    class RenderScene final
    {
      public:
        explicit RenderScene( const Framebuffer &framebuffer ) noexcept : m_Framebuffer{ framebuffer }
        {
        }

        // ---- Layer-aware API ----------------------------------------

        /**
         * Appends an empty Renderable to the given layer and returns a reference.
         * The caller must populate the returned object before Compile() runs.
         */
        Renderable &AddRenderable( const SceneLayer layer ) noexcept
        {
            auto &list = m_Layers[layer];
            list.PushBack( {} );
            return list.GetLastItem();
        }

        /**
         * Returns all renderables for a given layer.
         */
        const primitive::Vector< Renderable > &GetRenderables( const SceneLayer layer ) const noexcept
        {
            return m_Layers[layer];
        }

        // ---- Legacy / convenience (World layer) ---------------------

        /**
         * Adds a renderable to the World layer.
         * Preserved for backward compatibility with existing code.
         */
        Renderable &AddRenderable() noexcept
        {
            return AddRenderable( SceneLayer::World );
        }

        /**
         * Returns world-layer renderables.
         * Preserved for backward compatibility.
         */
        const primitive::Vector< Renderable > &GetRenderables() const noexcept
        {
            return m_Layers[SceneLayer::World];
        }

        // ---- View ---------------------------------------------------

        const View &GetView() const noexcept
        {
            return m_View;
        }

        View &GetView() noexcept
        {
            return m_View;
        }

        // ---- Framebuffer override -------------------------------------

        void SetFramebuffer( const Framebuffer &framebuffer ) noexcept
        {
            m_Framebuffer = framebuffer;
        }

        const Framebuffer &GetFramebuffer() const noexcept
        {
            return m_Framebuffer;
        }

        // ---- Frame lifecycle ----------------------------------------

        /**
         * Clears all layer lists. Call before each frame's scene extraction pass, after
         * the previous frame has finished rendering. The Renderer does not call this —
         * it is the caller's responsibility to clear before re-populating the scene.
         */
        void Clear() noexcept
        {
            for ( auto &list : m_Layers )
                list.Clear();
        }

      private:
        static constexpr Count s_LayerCount = static_cast< Count >( SceneLayer::Count );
        primitive::EnumArray< SceneLayer, primitive::Vector< Renderable > > m_Layers;
        View m_View;
        Framebuffer m_Framebuffer;
    };
}
