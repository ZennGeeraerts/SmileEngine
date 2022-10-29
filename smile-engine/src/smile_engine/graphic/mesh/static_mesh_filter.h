#pragma once

#include "mesh_filter.h"

namespace Smile::Graphic
{
    class StaticMeshFilter final : public MeshFilter
    {
      public:
        StaticMeshFilter() = default;
        virtual ~StaticMeshFilter();

        virtual void Create( const BufferLayout &layout ) override;

        virtual Ref< VertexBuffer > GetVertexBuffer() const override
        {
            return m_pVertexBuffer;
        }
        virtual Ref< IndexBuffer > GetIndexBuffer() const override
        {
            return m_pIndexBuffer;
        }
        virtual const std::string &GetFilePath() const override
        {
            return m_FilePath;
        }

        virtual void SetVertexCount( Uint32 vertexCount ) override
        {
            vertexCount = vertexCount;
        }
        virtual void AddPosition( const DirectX::XMFLOAT3 &position ) override;
        virtual void AddNormal( const DirectX::XMFLOAT3 &normal ) override;
        virtual void AddTangent( const DirectX::XMFLOAT3 &tangent ) override;
        virtual void AddBinormal( const DirectX::XMFLOAT3 &binormal ) override;
        virtual void AddTexCoord( const DirectX::XMFLOAT2 &texCoord ) override;
        virtual void AddColor( const DirectX::XMFLOAT4 &color ) override;

        virtual void SetIndexCount( Uint32 indexCount );
        virtual void AddIndex( Uint32 bufferPosition, Uint32 index ) override;

      private:
        std::vector< DirectX::XMFLOAT3 > m_Positions = {};
        std::vector< DirectX::XMFLOAT3 > m_Normals = {};
        std::vector< DirectX::XMFLOAT3 > m_Tangents = {};
        std::vector< DirectX::XMFLOAT3 > m_Binormals = {};
        std::vector< DirectX::XMFLOAT2 > m_TexCoords = {};
        std::vector< DirectX::XMFLOAT4 > m_Colors = {};

        bool m_UsePositions = false;
        bool m_UseNormals = false;
        bool m_UseTangents = false;
        bool m_UseBinormals = false;
        bool m_UseTexCoords = false;
        bool m_UseColors = false;

        std::vector< Uint32 > m_Indices = {};

        Ref< VertexBuffer > m_pVertexBuffer = nullptr;
        Ref< IndexBuffer > m_pIndexBuffer = nullptr;

        Uint32 m_VertexCount = 0;

        std::string m_FilePath = {};
        void *m_pDataLocation = nullptr;

        friend class MeshFactory;
        friend class MeshLoader;
    };
}
