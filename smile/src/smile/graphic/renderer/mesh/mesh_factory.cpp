/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "mesh_factory.h"

#include "smile/core/math/math.h"

namespace smile::graphic
{
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
    const DirectX::XMFLOAT3 MeshFactory::s_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
    const DirectX::XMFLOAT2 MeshFactory::s_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultIndices4 = DirectX::XMFLOAT4{ -1, -1, -1, -1 };

    static primitive::Vector< DirectX::XMFLOAT3 > s_PlanePositions{ { -1.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, 1.0f },
        { -1.0f, 0.0f, -1.0f } };

    static primitive::Vector< DirectX::XMFLOAT3 > s_PlaneNormals{ { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f } };

    static primitive::Vector< Uint32 > s_PlaneIndices{ 0, 1, 2, 2, 3, 0 };

    static primitive::Vector< DirectX::XMFLOAT3 > s_CubePositions{ // front
        { -0.5f, 0.5f, -0.5f },
        { 0.5f, 0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f },
        { 0.5f, -0.5f, -0.5f },

        // back
        { -0.5f, 0.5f, 0.5f },
        { 0.5f, 0.5f, 0.5f },
        { -0.5f, -0.5f, 0.5f },
        { 0.5f, -0.5f, 0.5f },

        // up
        { -0.5f, 0.5f, 0.5f },
        { 0.5f, 0.5f, 0.5f },
        { -0.5f, 0.5f, -0.5f },
        { 0.5f, 0.5f, -0.5f },

        // down
        { -0.5f, -0.5f, 0.5f },
        { 0.5f, -0.5f, 0.5f },
        { -0.5f, -0.5f, -0.5f },
        { 0.5f, -0.5f, -0.5f },

        // right
        { 0.5f, 0.5f, -0.5f },
        { 0.5f, 0.5f, 0.5f },
        { 0.5f, -0.5f, -0.5f },
        { 0.5f, -0.5f, 0.5f },

        // left
        { -0.5f, 0.5f, -0.5f },
        { -0.5f, 0.5f, 0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f, 0.5f } };

    static const primitive::Vector< Uint32 > s_CubeIndices{
        // front
        0 + 0,
        1 + 0,
        2 + 0,
        2 + 0,
        1 + 0,
        3 + 0,
        // back
        0 + 4,
        2 + 4,
        1 + 4,
        1 + 4,
        2 + 4,
        3 + 4,
        // up
        0 + 8,
        1 + 8,
        2 + 8,
        2 + 8,
        1 + 8,
        3 + 8,
        // down
        0 + 12,
        2 + 12,
        1 + 12,
        1 + 12,
        2 + 12,
        3 + 12,
        // right
        0 + 16,
        1 + 16,
        2 + 16,
        2 + 16,
        1 + 16,
        3 + 16,
        // left
        0 + 20,
        2 + 20,
        1 + 20,
        1 + 20,
        2 + 20,
        3 + 20,
    };

    static primitive::Vector< DirectX::XMFLOAT3 > s_CubeNormals{ // front
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },

        // back
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 0.0f, -1.0f },

        // up
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },

        // down
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f },

        // right
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },

        // left
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f } };

    Mesh MeshFactory::CreateMesh( const MeshSource::Ref &meshSource,
        const rhi::BufferLayout &layout,
        const MeshHandle handle ) const
    {
        meshSource->m_pDataLocation =
            malloc( static_cast< size_t >( layout.GetStride() ) * static_cast< size_t >( meshSource->m_VertexCount ) );
        if ( !meshSource->m_pDataLocation )
        {
            SM_LOG_ERROR( "MeshFactory::CreateMesh > Failed to allocate memory for the vertex buffer" );
            return {};
        }

        void *pData = meshSource->m_pDataLocation;

        for ( Index i{}; i < meshSource->m_VertexCount; ++i )
        {
            for ( const rhi::BufferElement &element : layout )
            {
                if ( element.Name == "POSITION" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::Positon ) ? &meshSource->m_Positions[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "NORMAL" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::Normal ) ? &meshSource->m_Normals[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "TEXCOORD" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::TexCoord ) ? &meshSource->m_TexCoords[i] : &s_DefaultFloat2,
                        element.Size );
                else if ( element.Name == "TANGENT" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::Tangent ) ? &meshSource->m_Tangents[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "BINORMAL" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::Binormal ) ? &meshSource->m_Binormals[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "COLOR" )
                    memcpy( meshSource->m_pDataLocation,
                        meshSource->HasSemantic( Semantic::Color ) ? &meshSource->m_Colors[i] : &s_DefaultFloat4,
                        element.Size );

                meshSource->m_pDataLocation = ( Byte * )meshSource->m_pDataLocation + element.Size;
            }
        }

        VertexBuffer vb = m_ResourceManager.CreateVertexBuffer( pData, meshSource->m_VertexCount, layout );
        IndexBuffer ib = m_ResourceManager.CreateIndexBuffer(
            meshSource->m_Indices.GetData(), meshSource->m_Indices.GetItemCount() );

        return Mesh{ handle, vb, ib };
    }

    SkinnedMesh MeshFactory::CreateSkinnedMesh( const SkinnedMeshSource::Ref &skinnedMeshSource,
        const rhi::BufferLayout &layout,
        const MeshHandle handle ) const
    {
        skinnedMeshSource->m_pDataLocation = malloc(
            static_cast< size_t >( layout.GetStride() ) * static_cast< size_t >( skinnedMeshSource->m_VertexCount ) );
        if ( !skinnedMeshSource->m_pDataLocation )
        {
            SM_LOG_ERROR( "MeshFactory::CreateSkinnedMesh > Failed to allocate memory for the vertex buffer" );
            return {};
        }

        void *pData = skinnedMeshSource->m_pDataLocation;

        for ( Index i{}; i < skinnedMeshSource->m_VertexCount; ++i )
        {
            for ( const rhi::BufferElement &element : layout )
            {
                if ( element.Name == "POSITION" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::Positon ) ? &skinnedMeshSource->m_Positions[i]
                                                                            : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "NORMAL" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::Normal ) ? &skinnedMeshSource->m_Normals[i]
                                                                           : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "TEXCOORD" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::TexCoord ) ? &skinnedMeshSource->m_TexCoords[i]
                                                                             : &s_DefaultFloat2,
                        element.Size );
                else if ( element.Name == "TANGENT" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::Tangent ) ? &skinnedMeshSource->m_Tangents[i]
                                                                            : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "BINORMAL" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::Binormal ) ? &skinnedMeshSource->m_Binormals[i]
                                                                             : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "COLOR" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::Color ) ? &skinnedMeshSource->m_Colors[i]
                                                                          : &s_DefaultFloat4,
                        element.Size );
                else if ( element.Name == "BLENDINDICES" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::BlendIndices ) ? &skinnedMeshSource->m_BlendIndices[i]
                                                                                 : &s_DefaultIndices4,
                        element.Size );
                else if ( element.Name == "BLENDWEIGHTS" )
                    memcpy( skinnedMeshSource->m_pDataLocation,
                        skinnedMeshSource->HasSemantic( Semantic::BlendWeights ) ? &skinnedMeshSource->m_BlendWeights[i]
                                                                                 : &s_DefaultFloat4,
                        element.Size );

                skinnedMeshSource->m_pDataLocation = ( Byte * )skinnedMeshSource->m_pDataLocation + element.Size;
            }
        }

        VertexBuffer vb = m_ResourceManager.CreateVertexBuffer( pData, skinnedMeshSource->m_VertexCount, layout );

        IndexBuffer ib = m_ResourceManager.CreateIndexBuffer(
            skinnedMeshSource->m_Indices.GetData(), skinnedMeshSource->m_Indices.GetItemCount() );

        return SkinnedMesh{ handle, vb, ib, skinnedMeshSource->m_Skeleton };
    }

    Mesh MeshFactory::CreatePlane( const rhi::BufferLayout &vertexLayout, const MeshHandle handle ) const
    {
        auto meshSource = memory::CreateRef< MeshSource >();

        for ( const auto &element : vertexLayout )
        {
            if ( element.Name == "POSITION" )
            {
                meshSource->m_Semantics.Set( Semantic::Positon );
                meshSource->m_Positions = s_PlanePositions;
            }
            else if ( element.Name == "NORMAL" )
            {
                meshSource->m_Semantics.Set( Semantic::Normal );
                meshSource->m_Normals = s_PlaneNormals;
            }
        }

        meshSource->m_VertexCount = s_PlanePositions.GetItemCount();
        meshSource->m_Indices = s_PlaneIndices;

        return CreateMesh( meshSource, vertexLayout, handle );
    }

    Mesh MeshFactory::CreateCube( const rhi::BufferLayout &vertexLayout, const MeshHandle handle ) const
    {
        auto meshSource = memory::CreateRef< MeshSource >();

        for ( const auto &element : vertexLayout )
        {
            if ( element.Name == "POSITION" )
            {
                meshSource->m_Semantics.Set( Semantic::Positon );
                meshSource->m_Positions = s_CubePositions;
            }
            else if ( element.Name == "NORMAL" )
            {
                meshSource->m_Semantics.Set( Semantic::Normal );
                meshSource->m_Normals = s_CubeNormals;
            }
            /*else if (element.Name == "TexCoord")
            {
                pStaticMeshFilter->m_bUseTexCoords = true;
                pStaticMeshFilter->m_TexCoords = m_CubeTexCoords;
            }*/
        }

        meshSource->m_VertexCount = s_CubePositions.GetItemCount();
        meshSource->m_Indices = s_CubeIndices;

        return CreateMesh( meshSource, vertexLayout, handle );
    }

    Mesh MeshFactory::CreateSphere( const rhi::BufferLayout &vertexLayout,
        const float radius,
        const Uint32 steps,
        const MeshHandle handle ) const
    {
        primitive::Vector< DirectX::XMFLOAT3 > positions{};
        primitive::Vector< DirectX::XMFLOAT3 > normals{};
        primitive::Vector< Uint32 > indices{};

        const auto vertCount = steps * ( steps - 1 ) + 2;

        // Vertices
        const float deltaTheta = math::g_PI / steps;
        const float deltaPhi = math::g_PI2 / steps;
        float theta = 0;
        float phi = 0;

        // TOP
        positions.EmplaceBack( 0.0f, radius, 0.0f );
        normals.EmplaceBack( 0.0f, 1.0f, 0.0f );

        // SPHERE
        for ( Index i{}; i < steps - 1; ++i )
        {
            theta += deltaTheta;
            for ( Index j{}; j < steps; ++j )
            {
                phi += deltaPhi;
                DirectX::XMFLOAT3 pos{};
                pos.x = radius * sin( theta ) * cos( phi );
                pos.z = radius * sin( theta ) * sin( phi );
                pos.y = radius * cos( theta );

                const DirectX::XMVECTOR vPos = XMLoadFloat3( &pos );
                DirectX::XMFLOAT3 normal;
                XMStoreFloat3( &normal, DirectX::XMVector3Normalize( vPos ) );

                positions.PushBack( std::move( pos ) );
                normals.PushBack( std::move( normal ) );
            }
        }

        // BOTTOM
        positions.EmplaceBack( 0.0f, -radius, 0.0f );
        normals.EmplaceBack( 0.0f, -1.0f, 0.0f );

        // Indices
        // TOP
        for ( Index i{}; i < steps + 1; ++i )
        {
            indices.PushBack( i );

            auto v1 = i + 1;
            if ( i % steps == 0 )
                v1 -= steps;

            indices.PushBack( v1 );
            indices.PushBack( 0 );
        }

        // MIDDLE
        for ( Index i{ 1 }; i < vertCount - 1 - steps; ++i )
        {
            const auto v0 = i;
            auto v1 = i + 1;

            if ( i % steps == 0 )
                v1 -= steps;

            const auto v2 = v1 + steps;
            const auto v3 = v0 + steps;

            indices.PushBack( v0 );
            indices.PushBack( v1 );
            indices.PushBack( v2 );
            indices.PushBack( v2 );
            indices.PushBack( v3 );
            indices.PushBack( v0 );
        }

        // BOTTOM
        for ( Index i{ vertCount - steps - 1 }; i < vertCount - 1; ++i )
        {
            indices.PushBack( i );

            auto v1 = i + 1;
            if ( i % steps == 0 )
                v1 -= steps;

            indices.PushBack( v1 );
            indices.PushBack( vertCount - 1 );
        }

        auto meshSource = memory::CreateRef< MeshSource >();

        for ( const auto &element : vertexLayout )
        {
            if ( element.Name == "POSITION" )
            {
                meshSource->m_Semantics.Set( Semantic::Positon );
                meshSource->m_Positions = positions;
            }
            else if ( element.Name == "NORMAL" )
            {
                meshSource->m_Semantics.Set( Semantic::Normal );
                meshSource->m_Normals = normals;
            }
        }

        meshSource->m_VertexCount = vertCount;
        meshSource->m_Indices = indices;

        return CreateMesh( meshSource, vertexLayout, handle );
    }
}