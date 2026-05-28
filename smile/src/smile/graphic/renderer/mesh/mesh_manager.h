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

#include "mesh_factory.h"

namespace smile::graphic
{
    class MeshManager final
    {
      public:
        explicit MeshManager( ResourceManager &resourceManager ) noexcept;

        Mesh CreateMesh( MeshSource::Ref meshSource );
        Mesh GetOrCreateMesh( MeshSource::Ref meshSource );

      private:
        primitive::HashMap< MeshSource::Ref, Mesh > m_MeshCache;

        MeshFactory m_MeshFactory;
        ResourceManager &m_ResourceManager;
    };
}