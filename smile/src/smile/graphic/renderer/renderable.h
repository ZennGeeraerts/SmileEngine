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
 * @file        renderable.h
 * @author      Zenn Geeraerts
 * @created     4 May 2026
 * @brief       Data that is used to render a scene from a specific point of view, such as a camera
 */
#pragma once

#include "smile/common/primitive/collection/vector.h"

#include "resource/vertex_buffer.h"
#include "resource/index_buffer.h"
#include "material/material.h"
#include "smile/graphic/rhi/primitive_topology.h"

namespace smile::graphic
{
    struct RenderPrimitive final
    {
        RenderPrimitive() = default;

        void
        SetGeometry( const VertexBuffer &vb, const IndexBuffer &ib, const rhi::PrimitiveTopology topology ) noexcept;

        void SetMaterialInstance( MaterialInstance materialInstance ) noexcept;

        VertexBuffer VertexBuffer;
        IndexBuffer IndexBuffer;
        MaterialInstance MaterialInstance;
        rhi::PrimitiveTopology Topology = rhi::PrimitiveTopology::None;
    };

    class Renderable final
    {
      public:
        RenderPrimitive &AddPrimitive() noexcept
        {
            m_Primitives.PushBack( {} );
            return m_Primitives.GetLastItem();
        }

        void SetGeometry( const Index primitiveIndex,
            const VertexBuffer &vb,
            const IndexBuffer &ib,
            const rhi::PrimitiveTopology topology );

        void SetMaterialInstance( const Index primitiveIndex, const MaterialInstance &materialInstance );

        void SetWorldTransform( const DirectX::XMFLOAT4X4 &worldTransform ) noexcept
        {
            m_WorldTransform = worldTransform;
        }

        const DirectX::XMFLOAT4X4 &GetWorldTransform() const noexcept
        {
            return m_WorldTransform;
        }

        auto begin() const noexcept
        {
            return m_Primitives.begin();
        }

        auto end() const noexcept
        {
            return m_Primitives.end();
        }

      private:
        primitive::Vector< RenderPrimitive > m_Primitives;
        DirectX::XMFLOAT4X4 m_WorldTransform;
    };
}