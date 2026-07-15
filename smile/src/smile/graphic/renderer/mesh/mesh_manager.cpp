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

    Mesh MeshManager::CreateMesh( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout )
    {
        const MeshHandle handle = m_HandleManager.CreateHandle();
        const Mesh mesh = m_MeshFactory.CreateMesh( meshSource, vertexLayout, handle );

        m_Meshes[handle.GetIndex()] = mesh;
        m_MeshCache.Add( meshSource, mesh );

        return mesh;
    }

    Mesh MeshManager::GetOrCreateMesh( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout )
    {
        const auto mesh = m_MeshCache.Find( meshSource );

        if ( mesh )
        {
            return *mesh;
        }

        return CreateMesh( meshSource, vertexLayout );
    }
}