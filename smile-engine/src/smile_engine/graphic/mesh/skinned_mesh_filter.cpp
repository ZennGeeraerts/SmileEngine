#include "smpch.h"
#include "skinned_mesh_filter.h"

namespace smile::graphic
{
    SkinnedMeshFilter::~SkinnedMeshFilter()
    {
        positions.clear();
        normals.clear();
        tangents.clear();
        binormals.clear();
        texCoords.clear();
        indices.clear();
        colors.clear();
        blendIndices.clear();
        blendWeights.clear();
        skeletonMap.clear();
    }

    void SkinnedMeshFilter::create( const BufferLayout &layout )
    {
        dataLocation = malloc( layout.getStride() * vertexCount );
        if ( !dataLocation )
        {
            SM_LOG_ERROR( "SkinnedMeshFilter::create > Failed to allocate memory for the vertex buffer" );
            return;
        }

        VertexBufferDescriptor vertex_buffer_data{};
        vertex_buffer_data.vertices = dataLocation;
        vertex_buffer_data.count = vertexCount;
        vertex_buffer_data.usage = BufferUsage::Immutable;
        vertex_buffer_data.stride = layout.getStride();

        for ( Uint32 i{}; i < vertexCount; ++i )
        {
            for ( const BufferElement &element : layout )
            {
                if ( element.name == "POSITION" )
                    memcpy( dataLocation, usePositions ? &positions[i] : &defaultFloat3, element.size );
                else if ( element.name == "NORMAL" )
                    memcpy( dataLocation, useNormals ? &normals[i] : &defaultFloat3, element.size );
                else if ( element.name == "TEXCOORD" )
                    memcpy( dataLocation, useTexCoords ? &texCoords[i] : &defaultFloat2, element.size );
                else if ( element.name == "TANGENT" )
                    memcpy( dataLocation, useTangents ? &tangents[i] : &defaultFloat3, element.size );
                else if ( element.name == "BINORMAL" )
                    memcpy( dataLocation, useBinormals ? &binormals[i] : &defaultFloat3, element.size );
                else if ( element.name == "COLOR" )
                    memcpy( dataLocation, useColors ? &colors[i] : &defaultFloat4, element.size );
                else if ( element.name == "BLENDINDICES" )
                    memcpy(
                        dataLocation, useBlendIndices ? &blendIndices[i] : &defaultIndices4, element.size );
                else if ( element.name == "BLENDWEIGHTS" )
                    memcpy( dataLocation, useBlendWeights ? &blendWeights[i] : &defaultFloat4, element.size );

                dataLocation = ( char * )dataLocation + element.size;
            }
        }

        IndexBufferDescriptor index_buffer_data{};
        index_buffer_data.indices = indices.data();
        index_buffer_data.count = static_cast< Uint32 >( indices.size() );
        index_buffer_data.usage = BufferUsage::Immutable;

        vertexBuffer.reset( VertexBuffer::create( vertex_buffer_data ) );
        indexBuffer.reset( IndexBuffer::create( index_buffer_data ) );
    }

    void SkinnedMeshFilter::addPosition( const DirectX::XMFLOAT3 &position )
    {
        usePositions = true;
        positions.push_back( position );
    }

    void SkinnedMeshFilter::addNormal( const DirectX::XMFLOAT3 &normal )
    {
        useNormals = true;
        normals.push_back( normal );
    }

    void SkinnedMeshFilter::addTangent( const DirectX::XMFLOAT3 &tangent )
    {
        useTangents = true;
        tangents.push_back( tangent );
    }

    void SkinnedMeshFilter::addBinormal( const DirectX::XMFLOAT3 &binormal )
    {
        useBinormals = true;
        binormals.push_back( binormal );
    }

    void SkinnedMeshFilter::addTexCoord( const DirectX::XMFLOAT2 &tex_coord )
    {
        useTexCoords = true;
        texCoords.push_back( tex_coord );
    }

    void SkinnedMeshFilter::addColor( const DirectX::XMFLOAT4 &color )
    {
        useColors = true;
        colors.push_back( color );
    }

    void SkinnedMeshFilter::setIndexCount( Uint32 index_count )
    {
        indices.resize( index_count );
    }

    void SkinnedMeshFilter::addIndex( Uint32 buffer_position, Uint32 index )
    {
        SM_ASSERT( buffer_position < indices.size(), "SkinnedMeshFilter::addIndex > Invalid buffer position" );
        indices[buffer_position] = index;
    }
}
