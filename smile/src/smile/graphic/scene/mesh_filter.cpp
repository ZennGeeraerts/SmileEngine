/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "mesh_filter.h"

namespace smile::graphic
{
    MeshFilter::~MeshFilter()
    {
        m_Positions.clear();
        m_Normals.clear();
        m_Tangents.clear();
        m_Binormals.clear();
        m_TexCoords.clear();
        m_Indices.clear();
        m_Colors.clear();
    }

    void MeshFilter::AddPosition( const DirectX::XMFLOAT3 &position )
    {
        AddSemantic( Semantic::Positon );
        m_Positions.push_back( position );
    }

    void MeshFilter::AddNormal( const DirectX::XMFLOAT3 &normal )
    {
        AddSemantic( Semantic::Normal );
        m_Normals.push_back( normal );
    }

    void MeshFilter::AddTangent( const DirectX::XMFLOAT3 &tangent )
    {
        AddSemantic( Semantic::Tangent );
        m_Tangents.push_back( tangent );
    }

    void MeshFilter::AddBinormal( const DirectX::XMFLOAT3 &binormal )
    {
        AddSemantic( Semantic::Binormal );
        m_Binormals.push_back( binormal );
    }

    void MeshFilter::AddTexCoord( const DirectX::XMFLOAT2 &texCoord )
    {
        AddSemantic( Semantic::TexCoord );
        m_TexCoords.push_back( texCoord );
    }

    void MeshFilter::AddColor( const DirectX::XMFLOAT4 &color )
    {
        AddSemantic( Semantic::Color );
        m_Colors.push_back( color );
    }

    void MeshFilter::SetIndexCount( Uint32 indexCount )
    {
        m_Indices.resize( indexCount );
    }

    void MeshFilter::AddIndex( Uint32 bufferPosition, Uint32 index )
    {
        SM_ASSERT_MSG( bufferPosition < m_Indices.size(), "MeshFilter::AddIndex > Invalid buffer position" );
        m_Indices[bufferPosition] = index;
    }
}