/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "static_mesh_filter.h"

#include "smile_engine/graphic/render_engine.h"

namespace smile::graphic
{
    StaticMeshFilter::~StaticMeshFilter()
    {
        m_Positions.clear();
        m_Normals.clear();
        m_Tangents.clear();
        m_Binormals.clear();
        m_TexCoords.clear();
        m_Indices.clear();
        m_Colors.clear();
    }

    void StaticMeshFilter::Create( const BufferLayout &layout )
    {
        m_pDataLocation =
            malloc( static_cast< size_t >( layout.GetStride() ) * static_cast< size_t >( m_VertexCount ) );
        if ( !m_pDataLocation )
        {
            SM_LOG_ERROR( "StaticMeshFilter::Create > Failed to allocate memory for the vertex buffer" );
            return;
        }

        VertexBufferDescriptor vertexBufferDesc{};
        vertexBufferDesc.pVertices = m_pDataLocation;
        vertexBufferDesc.Count = m_VertexCount;
        vertexBufferDesc.Usage = BufferUsage::Immutable;
        vertexBufferDesc.Stride = layout.GetStride();

        for ( Uint32 i{}; i < m_VertexCount; ++i )
        {
            for ( const BufferElement &element : layout )
            {
                if ( element.Name == "POSITION" )
                    memcpy( m_pDataLocation, m_UsePositions ? &m_Positions[i] : &s_DefaultFloat3, element.Size );
                else if ( element.Name == "NORMAL" )
                    memcpy( m_pDataLocation, m_UseNormals ? &m_Normals[i] : &s_DefaultFloat3, element.Size );
                else if ( element.Name == "TEXCOORD" )
                    memcpy( m_pDataLocation, m_UseTexCoords ? &m_TexCoords[i] : &s_DefaultFloat2, element.Size );
                else if ( element.Name == "TANGENT" )
                    memcpy( m_pDataLocation, m_UseTangents ? &m_Tangents[i] : &s_DefaultFloat3, element.Size );
                else if ( element.Name == "BINORMAL" )
                    memcpy( m_pDataLocation, m_UseBinormals ? &m_Binormals[i] : &s_DefaultFloat3, element.Size );
                else if ( element.Name == "COLOR" )
                    memcpy( m_pDataLocation, m_UseColors ? &m_Colors[i] : &s_DefaultFloat4, element.Size );

                m_pDataLocation = ( BYTE * )m_pDataLocation + element.Size;
            }
        }

        IndexBufferDescriptor indexBufferDesc{};
        indexBufferDesc.pIndices = m_Indices.data();
        indexBufferDesc.Count = static_cast< Uint32 >( m_Indices.size() );
        indexBufferDesc.Usage = BufferUsage::Immutable;

        GraphicsDevice *pDevice = RenderEngine::GetDevice();
        m_pVertexBuffer = pDevice->CreateVertexBuffer( vertexBufferDesc );
        m_pIndexBuffer = pDevice->CreateIndexBuffer( indexBufferDesc );
    }

    void StaticMeshFilter::AddPosition( const DirectX::XMFLOAT3 &position )
    {
        m_UsePositions = true;
        m_Positions.push_back( position );
    }

    void StaticMeshFilter::AddNormal( const DirectX::XMFLOAT3 &normal )
    {
        m_UseNormals = true;
        m_Normals.push_back( normal );
    }

    void StaticMeshFilter::AddTangent( const DirectX::XMFLOAT3 &tangent )
    {
        m_UseTangents = true;
        m_Tangents.push_back( tangent );
    }

    void StaticMeshFilter::AddBinormal( const DirectX::XMFLOAT3 &binormal )
    {
        m_UseBinormals = true;
        m_Binormals.push_back( binormal );
    }

    void StaticMeshFilter::AddTexCoord( const DirectX::XMFLOAT2 &texCoord )
    {
        m_UseTexCoords = true;
        m_TexCoords.push_back( texCoord );
    }

    void StaticMeshFilter::AddColor( const DirectX::XMFLOAT4 &color )
    {
        m_UseColors = true;
        m_Colors.push_back( color );
    }

    void StaticMeshFilter::SetIndexCount( Uint32 indexCount )
    {
        m_Indices.resize( indexCount );
    }

    void StaticMeshFilter::AddIndex( Uint32 bufferPosition, Uint32 index )
    {
        SM_ASSERT( bufferPosition < m_Indices.size(), "StaticMeshFilter::AddIndex > Invalid buffer position" );
        m_Indices[bufferPosition] = index;
    }
}