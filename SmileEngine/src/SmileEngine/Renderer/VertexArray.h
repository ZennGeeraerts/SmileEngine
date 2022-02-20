#pragma once

#include "Buffer.h"

namespace smile
{
    class VertexArray
    {
      public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) = 0;
        virtual void AddIndexBuffer( const Ref< IndexBuffer > &pVertexBuffer ) = 0;
        virtual const BufferLayout &GetLayout() const = 0;

        static VertexBuffer *Create();
    };
}