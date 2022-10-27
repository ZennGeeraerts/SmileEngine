#pragma once
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class MeshFilter
    {
      public:
        virtual void create( const BufferLayout &layout ) = 0;

        virtual Ref< VertexBuffer > getVertexBuffer() const = 0;
        virtual Ref< IndexBuffer > getIndexBuffer() const = 0;
        virtual const std::string &getFilePath() const = 0;

        virtual void setVertexCount( Uint32 vertex_count ) = 0;
        virtual void addPosition( const DirectX::XMFLOAT3 &position ) = 0;
        virtual void addNormal( const DirectX::XMFLOAT3 &normal ) = 0;
        virtual void addTangent( const DirectX::XMFLOAT3 &tangent ) = 0;
        virtual void addBinormal( const DirectX::XMFLOAT3 &binormal ) = 0;
        virtual void addTexCoord( const DirectX::XMFLOAT2 &tex_coord ) = 0;
        virtual void addColor( const DirectX::XMFLOAT4 &color ) = 0;

        virtual void setIndexCount( Uint32 index_count ) = 0;
        virtual void addIndex( Uint32 buffer_position, Uint32 index ) = 0;

      protected:
        static const DirectX::XMFLOAT4 defaultColor;
        static const DirectX::XMFLOAT4 defaultFloat4;
        static const DirectX::XMFLOAT3 defaultFloat3;
        static const DirectX::XMFLOAT2 defaultFloat2;
        static const DirectX::XMFLOAT4 defaultIndices4;
    };
}