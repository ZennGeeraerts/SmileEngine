#pragma once

#include "vertex_buffer.h"
#include "index_buffer.h"

namespace smile::graphic
{
    class VertexArray
    {
      public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer( const Ref< VertexBuffer > &vertex_buffer ) = 0;
        virtual void addIndexBuffer( const Ref< IndexBuffer > &index_buffer ) = 0;
        virtual const BufferLayout &getLayout() const = 0;

        static VertexBuffer *create();
    };
}