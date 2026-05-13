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
 * @file        renderable.cpp
 * @author      Zenn Geeraerts
 * @created     4 May 2026
 * @brief       Data that is used to render a scene from a specific point of view, such as a camera
 */
#include "smpch.h"
#include "renderable.h"

namespace smile::graphic
{
    void RenderPrimitive::SetGeometry( const graphic::VertexBuffer &vb,
        const graphic::IndexBuffer &ib,
        const rhi::PrimitiveTopology topology ) noexcept
    {
        VertexBuffer = vb;
        IndexBuffer = ib;
        Topology = topology;
    }

    void RenderPrimitive::SetMaterialInstance( graphic::MaterialInstance materialInstance ) noexcept
    {
        MaterialInstance = materialInstance;
    }

    void Renderable::SetGeometry( const Index primitiveIndex,
        const VertexBuffer &vb,
        const IndexBuffer &ib,
        const rhi::PrimitiveTopology topology )
    {
        SM_ASSERT_MSG( primitiveIndex < m_Primitives.GetItemCount(), "Primitive index out of bounds" );

        RenderPrimitive &primitive = m_Primitives[primitiveIndex];
        primitive.SetGeometry( vb, ib, topology );
    }

    void Renderable::SetMaterialInstance( const Index primitiveIndex, const MaterialInstance &materialInstance )
    {
        SM_ASSERT_MSG( primitiveIndex < m_Primitives.GetItemCount(), "Primitive index out of bounds" );

        RenderPrimitive &primitive = m_Primitives[primitiveIndex];
        primitive.SetMaterialInstance( materialInstance );
    }
}