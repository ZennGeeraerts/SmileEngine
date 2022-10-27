#pragma once

#include "mesh_filter.h"

namespace smile::graphic
{
    class StaticMeshFilter final : public MeshFilter
    {
      public:
        StaticMeshFilter() = default;
        virtual ~StaticMeshFilter();

        virtual void create( const BufferLayout &layout ) override;

        virtual Ref< VertexBuffer > getVertexBuffer() const override
        {
            return vertexBuffer;
        }
        virtual Ref< IndexBuffer > getIndexBuffer() const override
        {
            return indexBuffer;
        }
        virtual const std::string &getFilePath() const override
        {
            return filePath;
        }

        virtual void setVertexCount( Uint32 vertex_count ) override
        {
            vertexCount = vertex_count;
        }
        virtual void addPosition( const DirectX::XMFLOAT3 &position ) override;
        virtual void addNormal( const DirectX::XMFLOAT3 &normal ) override;
        virtual void addTangent( const DirectX::XMFLOAT3 &tangent ) override;
        virtual void addBinormal( const DirectX::XMFLOAT3 &binormal ) override;
        virtual void addTexCoord( const DirectX::XMFLOAT2 &tex_coord ) override;
        virtual void addColor( const DirectX::XMFLOAT4 &color ) override;

        virtual void setIndexCount( Uint32 index_count );
        virtual void addIndex( Uint32 buffer_position, Uint32 index ) override;

      private:
        std::vector< DirectX::XMFLOAT3 > positions = {};
        std::vector< DirectX::XMFLOAT3 > normals = {};
        std::vector< DirectX::XMFLOAT3 > tangents = {};
        std::vector< DirectX::XMFLOAT3 > binormals = {};
        std::vector< DirectX::XMFLOAT2 > texCoords = {};
        std::vector< DirectX::XMFLOAT4 > colors = {};

        bool usePositions = false;
        bool useNormals = false;
        bool useTangents = false;
        bool useBinormals = false;
        bool useTexCoords = false;
        bool useColors = false;

        std::vector< Uint32 > indices = {};

        Ref< VertexBuffer > vertexBuffer = nullptr;
        Ref< IndexBuffer > indexBuffer = nullptr;

        Uint32 vertexCount = 0;

        std::string filePath = {};
        void *dataLocation = nullptr;

        friend class MeshFactory;
        friend class MeshLoader;
    };
}
