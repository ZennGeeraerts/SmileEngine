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

#include <variant>

namespace smile::graphic
{
    static constexpr Uint16 s_MaxMeshCount = ( 12 << 10 );

    using MeshSlotMap = typename primitive::SlotMap< Mesh, s_MaxMeshCount, Uint64, 32u, 32u, struct Mesh >;
    using MeshHandle = MeshSlotMap::HandleType;

    enum class PrimitiveMeshShape
    {
        Cube,
        Sphere,
        Plane
    };

    using MeshGeometry = std::variant< PrimitiveMeshShape, MeshSource::Ref >;

    struct MeshKey final
    {
        MeshKey( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout ) noexcept
            : Geometry{ std::move( meshSource ) }, VertexLayout{ vertexLayout }
        {
        }

        MeshKey( PrimitiveMeshShape shape, const rhi::BufferLayout &vertexLayout ) noexcept
            : Geometry{ shape }, VertexLayout{ vertexLayout }
        {
        }

        bool operator==( const MeshKey &other ) const noexcept
        {
            return Geometry == other.Geometry && VertexLayout == other.VertexLayout;
        }

        bool operator!=( const MeshKey &other ) const noexcept
        {
            return !( *this == other );
        }

        foundation::HashCode GetHashCode() const noexcept
        {
            foundation::HashCode hash = std::visit( []( const auto &geometry )
                { return std::hash< std::decay_t< decltype( geometry ) > >{}( geometry ); },
                Geometry );

            hash = foundation::HashCombine( hash, VertexLayout.GetHashCode() );

            return hash;
        }

        MeshGeometry Geometry;
        rhi::BufferLayout VertexLayout;
    };

    class MeshManager final
    {
      public:
        explicit MeshManager( ResourceManager &resourceManager ) noexcept;

        MeshHandle CreateMesh( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout );
        MeshHandle CreateMeshIfNotExists( MeshSource::Ref meshSource, const rhi::BufferLayout &vertexLayout );

        MeshHandle CreateCube( const rhi::BufferLayout &vertexLayout );
        MeshHandle CreateCubeIfNotExists( const rhi::BufferLayout &vertexLayout );

        const Mesh &GetMesh( MeshHandle meshHandle ) const;

      private:
        MeshSlotMap m_Meshes;
        ResourceCache< MeshKey, MeshHandle > m_MeshCache;

        MeshFactory m_MeshFactory;
        ResourceManager &m_ResourceManager;
    };
}

namespace std
{
    template <>
    struct hash< smile::graphic::MeshKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::MeshKey &key ) const noexcept
        {
            return key.GetHashCode();
        }
    };
}