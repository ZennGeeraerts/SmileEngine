#include "smpch.h"
#include "static_mesh_filter.h"

namespace smile
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
        m_pDataLocation = malloc( layout.GetStride() * m_VertexCount );
        if ( !m_pDataLocation )
        {
            SM_LOG_ERROR( "StaticMeshFilter::Create > Failed to allocate memory for the vertex buffer" );
            return;
        }

        VertexBufferDescriptor vertexBufferData{};
        vertexBufferData.m_pVertices = m_pDataLocation;
        vertexBufferData.m_Count = m_VertexCount;
        vertexBufferData.m_Usage = BufferUsage::Immutable;
        vertexBufferData.m_Stride = layout.GetStride();

        for ( Uint32 i{}; i < m_VertexCount; ++i )
        {
            for ( const BufferElement &element : layout )
            {
                if ( element.m_Name == "POSITION" )
                    memcpy( m_pDataLocation, m_bUsePositions ? &m_Positions[i] : &s_DefaultFloat3, element.m_Size );
                else if ( element.m_Name == "NORMAL" )
                    memcpy( m_pDataLocation, m_bUseNormals ? &m_Normals[i] : &s_DefaultFloat3, element.m_Size );
                else if ( element.m_Name == "TEXCOORD" )
                    memcpy( m_pDataLocation, m_bUseTexCoords ? &m_TexCoords[i] : &s_DefaultFloat2, element.m_Size );
                else if ( element.m_Name == "TANGENT" )
                    memcpy( m_pDataLocation, m_bUseTangents ? &m_Tangents[i] : &s_DefaultFloat3, element.m_Size );
                else if ( element.m_Name == "BINORMAL" )
                    memcpy( m_pDataLocation, m_bUseBinormals ? &m_Binormals[i] : &s_DefaultFloat3, element.m_Size );
                else if ( element.m_Name == "COLOR" )
                    memcpy( m_pDataLocation, m_bUseColors ? &m_Colors[i] : &s_DefaultFloat4, element.m_Size );

                m_pDataLocation = ( BYTE * )m_pDataLocation + element.m_Size;
            }
        }

        IndexBufferDescriptor indexBufferData{};
        indexBufferData.m_pIndices = m_Indices.data();
        indexBufferData.m_Count = static_cast< uint32_t >( m_Indices.size() );
        indexBufferData.m_Usage = BufferUsage::Immutable;

        m_pVertexBuffer.reset( VertexBuffer::Create( vertexBufferData ) );
        m_pIndexBuffer.reset( IndexBuffer::Create( indexBufferData ) );
    }

    void StaticMeshFilter::AddPosition( const DirectX::XMFLOAT3 &position )
    {
        m_bUsePositions = true;
        m_Positions.push_back( position );
    }

    void StaticMeshFilter::AddNormal( const DirectX::XMFLOAT3 &normal )
    {
        m_bUseNormals = true;
        m_Normals.push_back( normal );
    }

    void StaticMeshFilter::AddTangent( const DirectX::XMFLOAT3 &tangent )
    {
        m_bUseTangents = true;
        m_Tangents.push_back( tangent );
    }

    void StaticMeshFilter::AddBinormal( const DirectX::XMFLOAT3 &binormal )
    {
        m_bUseBinormals = true;
        m_Binormals.push_back( binormal );
    }

    void StaticMeshFilter::AddTexCoord( const DirectX::XMFLOAT2 &texCoord )
    {
        m_bUseTexCoords = true;
        m_TexCoords.push_back( texCoord );
    }

    void StaticMeshFilter::AddColor( const DirectX::XMFLOAT4 &color )
    {
        m_bUseColors = true;
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