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
 * @file        mesh_manager.h
 * @author      Zenn Geeraerts
 * @created     28 May 2026
 * @brief       Manages mesh assets and their GPU resources
 */
#pragma once

#include "smile/common/primitive/collection/slot_map.h"
#include "mesh_factory.h"
#include "smile/graphic/renderer/resource/resource_cache.h"

namespace smile::graphic
{
    static constexpr Uint16 s_MaxMeshCount = ( 12 << 10 );

    using MeshSlotMap = typename primitive::SlotMap< Mesh, s_MaxMeshCount, Uint64, 32u, 32u, struct Mesh >;
    using MeshHandle = MeshSlotMap::HandleType;

    class MeshManager final
    {
      public:
        explicit MeshManager( ResourceManager &resourceManager ) noexcept;

        MeshHandle CreateMesh( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout );
        MeshHandle CreateMeshIfNotExists( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout );
        const Mesh &GetMesh( MeshHandle meshHandle ) const;

      private:
        MeshSlotMap m_Meshes;
        ResourceCache< MeshSource::Ref, MeshHandle > m_MeshCache;

        MeshFactory m_MeshFactory;
        ResourceManager &m_ResourceManager;
    };
}