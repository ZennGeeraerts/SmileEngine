#pragma once
#include "smile_engine/graphic/resource/vertex_buffer.h"
#include "smile_engine/graphic/resource/index_buffer.h"

#include <DirectXMath.h>

namespace smile::graphic
{
    class MeshFilter
    {
      public:
        virtual void Create( const BufferLayout &layout ) = 0;

        virtual Ref< VertexBuffer > GetVertexBuffer() const = 0;
        virtual Ref< IndexBuffer > GetIndexBuffer() const = 0;
        virtual const std::string &GetFilePath() const = 0;

        virtual void SetVertexCount( Uint32 vertexCount ) = 0;
        virtual void AddPosition( const DirectX::XMFLOAT3 &position ) = 0;
        virtual void AddNormal( const DirectX::XMFLOAT3 &normal ) = 0;
        virtual void AddTangent( const DirectX::XMFLOAT3 &tangent ) = 0;
        virtual void AddBinormal( const DirectX::XMFLOAT3 &binormal ) = 0;
        virtual void AddTexCoord( const DirectX::XMFLOAT2 &texCoord ) = 0;
        virtual void AddColor( const DirectX::XMFLOAT4 &color ) = 0;

        virtual void SetIndexCount( Uint32 indexCount ) = 0;
        virtual void AddIndex( Uint32 bufferPosition, Uint32 index ) = 0;

      protected:
        static const DirectX::XMFLOAT4 s_DefaultColor;
        static const DirectX::XMFLOAT4 s_DefaultFloat4;
        static const DirectX::XMFLOAT3 s_DefaultFloat3;
        static const DirectX::XMFLOAT2 s_DefaultFloat2;
        static const DirectX::XMFLOAT4 s_DefaultIndices4;
    };
}