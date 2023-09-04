/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "mesh_factory.h"

#include "smile_engine/graphic/render_engine.h"

namespace smile::graphic
{
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultColor = DirectX::XMFLOAT4{ 1, 0, 0, 1 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultFloat4 = DirectX::XMFLOAT4{ 0, 0, 0, 0 };
    const DirectX::XMFLOAT3 MeshFactory::s_DefaultFloat3 = DirectX::XMFLOAT3{ 0, 0, 0 };
    const DirectX::XMFLOAT2 MeshFactory::s_DefaultFloat2 = DirectX::XMFLOAT2{ 0, 0 };
    const DirectX::XMFLOAT4 MeshFactory::s_DefaultIndices4 = DirectX::XMFLOAT4{ -1, -1, -1, -1 };

    static std::vector< DirectX::XMFLOAT3 > s_CubePositions
    { 
        // front
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },

        // back
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },

        // up
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },

        // down
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },

        // right
        { 1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, 1.0f },

        // left
        { -1.0f, 1.0f, -1.0f },
        { -1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f } 
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

        GraphicsDevice *pDevice = RenderEngine::GetDevice();
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

        GraphicsDevice *pDevice = RenderEngine::GetDevice();
        pSkinnedMesh->pVertexBuffer = pDevice->CreateVertexBuffer( vertexBufferDesc );
        pSkinnedMesh->pIndexBuffer = pDevice->CreateIndexBuffer( indexBufferDesc );
        pSkinnedMesh->SkeletonMap = pSkinnedMeshFilter->m_SkeletonMap;
        pSkinnedMesh->BoneCount = pSkinnedMeshFilter->m_BoneCount;

        return pSkinnedMesh;
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
}