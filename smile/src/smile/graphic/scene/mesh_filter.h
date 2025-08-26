/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <DirectXMath.h>

namespace smile::graphic
{
    enum class Semantic : Uint32
    {
        None = 0,
        Positon = BIT( 0 ),
        Normal = BIT( 1 ),
        TexCoord = BIT( 2 ),
        Binormal = BIT( 3 ),
        Tangent = BIT( 4 ),
        Color = BIT( 5 ),
        BlendIndices = BIT( 6 ),
        BlendWeights = BIT( 7 )
    };

    class MeshFilter
    {
      public:
        MeshFilter() = default;
        virtual ~MeshFilter();

        const std::string& GetName() const
        {
            return m_Name;
        }

        void SetVertexCount( Uint32 vertexCount )
        {
            m_VertexCount = vertexCount;
        }
        void AddPosition( const DirectX::XMFLOAT3 &position );
        void AddNormal( const DirectX::XMFLOAT3 &normal );
        void AddTangent( const DirectX::XMFLOAT3 &tangent );
        void AddBinormal( const DirectX::XMFLOAT3 &binormal );
        void AddTexCoord( const DirectX::XMFLOAT2 &texCoord );
        void AddColor( const DirectX::XMFLOAT4 &color );

        void SetIndexCount( Uint32 indexCount );
        void AddIndex( Uint32 bufferPosition, Uint32 index );

        bool HasSemantic( Semantic semantic )
        {
            return ( m_Semantics & static_cast< Uint32 >( semantic ) ) > 0;
        }

      private:
        void AddSemantic( Semantic semantic )
        {
            m_Semantics |= static_cast< Uint32 >( semantic );
        }

      private:
        std::string m_Name;
        std::vector< DirectX::XMFLOAT3 > m_Positions = {};
        std::vector< DirectX::XMFLOAT3 > m_Normals = {};
        std::vector< DirectX::XMFLOAT3 > m_Tangents = {};
        std::vector< DirectX::XMFLOAT3 > m_Binormals = {};
        std::vector< DirectX::XMFLOAT2 > m_TexCoords = {};
        std::vector< DirectX::XMFLOAT4 > m_Colors = {};

        Uint32 m_Semantics = static_cast< Uint32 >( Semantic::None );

        std::vector< Uint32 > m_Indices = {};

        Uint32 m_VertexCount = 0;

        void *m_pDataLocation = nullptr;

        friend class MeshFactory;
        friend class ModelLoader;
    };
}