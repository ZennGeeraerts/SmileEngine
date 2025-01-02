/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "mesh_factory.h"

#include "smile/engine/graphic/renderer/render_command.h"
#include "smile/engine/core/math/math.h"

namespace smile::graphic
{
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
    const DirectX::XMFLOAT3 MeshFactory::s_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
    const DirectX::XMFLOAT2 MeshFactory::s_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultIndices4 = DirectX::XMFLOAT4{ -1, -1, -1, -1 };

    static std::vector< DirectX::XMFLOAT3 > s_PlanePositions
    {
        { -1.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, -1.0f },
        { 1.0f, 0.0f, 1.0f },
        { -1.0f, 0.0f, -1.0f } 
    };

    static std::vector< DirectX::XMFLOAT3 > s_PlaneNormals
    {
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }
    };

    static std::vector< Uint32 > s_PlaneIndices
    {
        0, 1, 2,
        2, 3, 0
    };

    static std::vector< DirectX::XMFLOAT3 > s_CubePositions
    { 
        // front
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
        { -0.5f, -0.5f, 0.5f }
    };

    static const std::vector< Uint32 > s_CubeIndices
    {
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

    static std::vector< DirectX::XMFLOAT3 > s_CubeNormals
    { 
        // front
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
        { -1.0f, 0.0f, 0.0f } 
    };

    Ref< Mesh > MeshFactory::CreateMesh( const Ref< MeshFilter > &pMeshFilter, const BufferLayout &layout )
    {
        pMeshFilter->m_pDataLocation =
            malloc( static_cast< size_t >( layout.GetStride() ) * static_cast< size_t >( pMeshFilter->m_VertexCount ) );
        if ( !pMeshFilter->m_pDataLocation )
        {
            SM_LOG_ERROR( "MeshFactory::CreateMesh > Failed to allocate memory for the vertex buffer" );
            return nullptr;
        }

        VertexBufferDescriptor vertexBufferDesc{};
        vertexBufferDesc.pVertices = pMeshFilter->m_pDataLocation;
        vertexBufferDesc.Count = pMeshFilter->m_VertexCount;
        vertexBufferDesc.Usage = BufferUsage::Immutable;
        vertexBufferDesc.Stride = layout.GetStride();

        for ( Uint32 i{}; i < pMeshFilter->m_VertexCount; ++i )
        {
            for ( const BufferElement &element : layout )
            {
                if ( element.Name == "POSITION" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::Positon ) ? &pMeshFilter->m_Positions[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "NORMAL" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::Normal ) ? &pMeshFilter->m_Normals[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "TEXCOORD" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::TexCoord ) ? &pMeshFilter->m_TexCoords[i]
                                                                       : &s_DefaultFloat2,
                        element.Size );
                else if ( element.Name == "TANGENT" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::Tangent ) ? &pMeshFilter->m_Tangents[i] : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "BINORMAL" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::Binormal ) ? &pMeshFilter->m_Binormals[i]
                                                                       : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "COLOR" )
                    memcpy( pMeshFilter->m_pDataLocation,
                        pMeshFilter->HasSemantic( Semantic::Color ) ? &pMeshFilter->m_Colors[i] : &s_DefaultFloat4,
                        element.Size );

                pMeshFilter->m_pDataLocation = ( Byte * )pMeshFilter->m_pDataLocation + element.Size;
            }
        }

        IndexBufferDescriptor indexBufferDesc{};
        indexBufferDesc.pIndices = pMeshFilter->m_Indices.data();
        indexBufferDesc.Count = static_cast< Uint32 >( pMeshFilter->m_Indices.size() );
        indexBufferDesc.Usage = BufferUsage::Immutable;

        Ref< Mesh > pMesh = CreateRef< Mesh >();

        GraphicsDevice *pDevice = RenderCommand::GetGraphicsDevice();
        pMesh->pVertexBuffer = pDevice->CreateVertexBuffer( vertexBufferDesc );
        pMesh->pIndexBuffer = pDevice->CreateIndexBuffer( indexBufferDesc );

        return pMesh;
    }

    Ref< SkinnedMesh > MeshFactory::CreateSkinnedMesh( const Ref< SkinnedMeshFilter > &pSkinnedMeshFilter,
        const BufferLayout &layout )
    {
        pSkinnedMeshFilter->m_pDataLocation = malloc(
            static_cast< size_t >( layout.GetStride() ) * static_cast< size_t >( pSkinnedMeshFilter->m_VertexCount ) );
        if ( !pSkinnedMeshFilter->m_pDataLocation )
        {
            SM_LOG_ERROR( "SkinnedMeshFilter::Create > Failed to allocate memory for the vertex buffer" );
            return nullptr;
        }

        VertexBufferDescriptor vertexBufferDesc{};
        vertexBufferDesc.pVertices = pSkinnedMeshFilter->m_pDataLocation;
        vertexBufferDesc.Count = pSkinnedMeshFilter->m_VertexCount;
        vertexBufferDesc.Usage = BufferUsage::Immutable;
        vertexBufferDesc.Stride = layout.GetStride();

        for ( Uint32 i{}; i < pSkinnedMeshFilter->m_VertexCount; ++i )
        {
            for ( const BufferElement &element : layout )
            {
                if ( element.Name == "POSITION" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::Positon ) ? &pSkinnedMeshFilter->m_Positions[i]
                                                                             : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "NORMAL" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::Normal ) ? &pSkinnedMeshFilter->m_Normals[i]
                                                                            : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "TEXCOORD" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::TexCoord ) ? &pSkinnedMeshFilter->m_TexCoords[i]
                                                                              : &s_DefaultFloat2,
                        element.Size );
                else if ( element.Name == "TANGENT" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::Tangent ) ? &pSkinnedMeshFilter->m_Tangents[i]
                                                                             : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "BINORMAL" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::Binormal ) ? &pSkinnedMeshFilter->m_Binormals[i]
                                                                              : &s_DefaultFloat3,
                        element.Size );
                else if ( element.Name == "COLOR" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::Color ) ? &pSkinnedMeshFilter->m_Colors[i]
                                                                           : &s_DefaultFloat4,
                        element.Size );
                else if ( element.Name == "BLENDINDICES" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::BlendIndices )
                            ? &pSkinnedMeshFilter->m_BlendIndices[i]
                            : &s_DefaultIndices4,
                        element.Size );
                else if ( element.Name == "BLENDWEIGHTS" )
                    memcpy( pSkinnedMeshFilter->m_pDataLocation,
                        pSkinnedMeshFilter->HasSemantic( Semantic::BlendWeights )
                            ? &pSkinnedMeshFilter->m_BlendWeights[i]
                            : &s_DefaultFloat4,
                        element.Size );

                pSkinnedMeshFilter->m_pDataLocation = ( Byte * )pSkinnedMeshFilter->m_pDataLocation + element.Size;
            }
        }

        IndexBufferDescriptor indexBufferDesc{};
        indexBufferDesc.pIndices = pSkinnedMeshFilter->m_Indices.data();
        indexBufferDesc.Count = static_cast< Uint32 >( pSkinnedMeshFilter->m_Indices.size() );
        indexBufferDesc.Usage = BufferUsage::Immutable;

        Ref< SkinnedMesh > pSkinnedMesh = CreateRef< SkinnedMesh >();

        GraphicsDevice *pDevice = RenderCommand::GetGraphicsDevice();
        pSkinnedMesh->pVertexBuffer = pDevice->CreateVertexBuffer( vertexBufferDesc );
        pSkinnedMesh->pIndexBuffer = pDevice->CreateIndexBuffer( indexBufferDesc );
        pSkinnedMesh->SkeletonMap = pSkinnedMeshFilter->m_SkeletonMap;
        pSkinnedMesh->BoneCount = pSkinnedMeshFilter->m_BoneCount;

        return pSkinnedMesh;
    }

    Ref< Mesh > MeshFactory::CreatePlane( const BufferLayout &bufferLayout )
    {
        Ref< MeshFilter > pMeshFilter = CreateRef< MeshFilter >();

        for ( const auto &element : bufferLayout )
        {
            if ( element.Name == "POSITION" )
            {
                pMeshFilter->AddSemantic( Semantic::Positon );
                pMeshFilter->m_Positions = s_PlanePositions;
            }
            else if ( element.Name == "NORMAL" )
            {
                pMeshFilter->AddSemantic( Semantic::Normal );
                pMeshFilter->m_Normals = s_PlaneNormals;
            }
        }

        pMeshFilter->m_VertexCount = static_cast< Uint32 >( s_PlanePositions.size() );
        pMeshFilter->m_Indices = s_PlaneIndices;

        return CreateMesh( pMeshFilter, bufferLayout );
    }

    Ref< Mesh > MeshFactory::CreateCube( const BufferLayout &bufferLayout )
    {
        Ref< MeshFilter > pMeshFilter = CreateRef< MeshFilter >();

        for ( const auto &element : bufferLayout )
        {
            if ( element.Name == "POSITION" )
            {
                pMeshFilter->AddSemantic( Semantic::Positon );
                pMeshFilter->m_Positions = s_CubePositions;
            }
            else if ( element.Name == "NORMAL" )
            {
                pMeshFilter->AddSemantic( Semantic::Normal );
                pMeshFilter->m_Normals = s_CubeNormals;
            }
            /*else if (element.Name == "TexCoord")
            {
                pStaticMeshFilter->m_bUseTexCoords = true;
                pStaticMeshFilter->m_TexCoords = m_CubeTexCoords;
            }*/
        }

        pMeshFilter->m_VertexCount = static_cast< Uint32 >( s_CubePositions.size() );
        pMeshFilter->m_Indices = s_CubeIndices;

        return CreateMesh( pMeshFilter, bufferLayout );
    }

    Ref< Mesh > MeshFactory::CreateSphere( const BufferLayout &bufferLayout, const float radius, const Uint32 steps )
    {
        std::vector< DirectX::XMFLOAT3 > positions{};
        std::vector< DirectX::XMFLOAT3 > normals{};
        std::vector< Uint32 > indices{};

        const auto vertCount = steps * ( steps - 1 ) + 2;

        // Vertices
        const float deltaTheta = math::g_PI / steps;
        const float deltaPhi = math::g_PI2 / steps;
        float theta = 0;
        float phi = 0;

        // TOP
        positions.push_back( { 0, radius, 0 } );
        normals.push_back( { 0, 1, 0 } );

        // SPHERE
        for ( Uint32 i{}; i < steps - 1; ++i )
        {
            theta += deltaTheta;
            for ( Uint32 j{}; j < steps; ++j )
            {
                phi += deltaPhi;
                DirectX::XMFLOAT3 pos{};
                pos.x = radius * sin( theta ) * cos( phi );
                pos.z = radius * sin( theta ) * sin( phi );
                pos.y = radius * cos( theta );

                const DirectX::XMVECTOR vPos = XMLoadFloat3( &pos );
                DirectX::XMFLOAT3 normal;
                XMStoreFloat3( &normal, DirectX::XMVector3Normalize( vPos ) );

                positions.push_back( pos );
                normals.push_back( normal );
            }
        }

        // BOTTOM
        positions.push_back( { 0, -radius, 0 } );
        normals.push_back( { 0, -1, 0 } );

        // Indices
        // TOP
        for ( Uint32 i{}; i < steps + 1; ++i )
        {
            indices.push_back( i );

            auto v1 = i + 1;
            if ( i % steps == 0 )
                v1 -= steps;

            indices.push_back( v1 );
            indices.push_back( 0 );
        }

        // MIDDLE
        for ( Uint32 i{ 1 }; i < vertCount - 1 - steps; ++i )
        {
            const auto v0 = i;
            auto v1 = i + 1;

            if ( i % steps == 0 )
                v1 -= steps;

            const auto v2 = v1 + steps;
            const auto v3 = v0 + steps;

            indices.push_back( v0 );
            indices.push_back( v1 );
            indices.push_back( v2 );
            indices.push_back( v2 );
            indices.push_back( v3 );
            indices.push_back( v0 );
        }

        // BOTTOM
        for ( Uint32 i{ vertCount - steps - 1 }; i < vertCount - 1; ++i )
        {
            indices.push_back( i );

            auto v1 = i + 1;
            if ( i % steps == 0 )
                v1 -= steps;

            indices.push_back( v1 );
            indices.push_back( vertCount - 1 );
        }

        Ref< MeshFilter > pMeshFilter = CreateRef< MeshFilter >();

        for ( const auto &element : bufferLayout )
        {
            if ( element.Name == "POSITION" )
            {
                pMeshFilter->AddSemantic( Semantic::Positon );
                pMeshFilter->m_Positions = positions;
            }
            else if ( element.Name == "NORMAL" )
            {
                pMeshFilter->AddSemantic( Semantic::Normal );
                pMeshFilter->m_Normals = normals;
            }
        }

        pMeshFilter->m_VertexCount = vertCount;
        pMeshFilter->m_Indices = indices;

        return CreateMesh( pMeshFilter, bufferLayout );
    }
}