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
 * @file        mesh_manager.cpp
 * @author      Zenn Geeraerts
 * @created     28 May 2026
 * @brief       Manages mesh assets and their GPU resources
 */
#include "smpch.h"
#include "mesh_manager.h"

namespace smile::graphic
{
    MeshManager::MeshManager( ResourceManager &resourceManager ) noexcept
        : m_MeshFactory{ resourceManager }, m_ResourceManager{ resourceManager }
    {
    }

    MeshHandle MeshManager::CreateMesh( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout )
    {
        const Mesh mesh = m_MeshFactory.CreateMesh( meshSource, vertexLayout );

        const auto meshHandle = m_Meshes.Insert( mesh );
        m_MeshCache.Add( meshSource, meshHandle );

        return meshHandle;
    }

    MeshHandle MeshManager::CreateMeshIfNotExists( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout )
    {
        const auto meshHandle = m_MeshCache.Find( meshSource );

        if ( meshHandle )
        {
            return *meshHandle;
        }

        return CreateMesh( meshSource, vertexLayout );
    }

    const Mesh &MeshManager::GetMesh( MeshHandle meshHandle ) const
    {
        return m_Meshes.GetItemAtSlot( meshHandle );
    }
}