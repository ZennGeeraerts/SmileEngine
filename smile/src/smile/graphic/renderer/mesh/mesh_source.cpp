/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "mesh_source.h"

namespace smile::graphic
{
    void MeshSource::AddPosition( const DirectX::XMFLOAT3 &position )
    {
        m_Semantics.Set( Semantic::Positon );
        m_Positions.PushBack( position );
    }

    void MeshSource::AddNormal( const DirectX::XMFLOAT3 &normal )
    {
        m_Semantics.Set( Semantic::Normal );
        m_Normals.PushBack( normal );
    }

    void MeshSource::AddTangent( const DirectX::XMFLOAT3 &tangent )
    {
        m_Semantics.Set( Semantic::Tangent );
        m_Tangents.PushBack( tangent );
    }

    void MeshSource::AddBinormal( const DirectX::XMFLOAT3 &binormal )
    {
        m_Semantics.Set( Semantic::Binormal );
        m_Binormals.PushBack( binormal );
    }

    void MeshSource::AddTexCoord( const DirectX::XMFLOAT2 &texCoord )
    {
        m_Semantics.Set( Semantic::TexCoord );
        m_TexCoords.PushBack( texCoord );
    }

    void MeshSource::AddColor( const DirectX::XMFLOAT4 &color )
    {
        m_Semantics.Set( Semantic::Color );
        m_Colors.PushBack( color );
    }

    void MeshSource::SetIndexCount( Count indexCount )
    {
        m_Indices.SetItemCount( indexCount );
    }

    void MeshSource::AddIndex( Index bufferPosition, Index index )
    {
        SM_ASSERT_MSG( bufferPosition < m_Indices.GetItemCount(), "MeshFilter::AddIndex > Invalid buffer position" );
        m_Indices[bufferPosition] = index;
    }
}