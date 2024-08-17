/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "vertex_buffer.h"
#include "index_buffer.h"

namespace smile::graphic
{
    class VertexArray
    {
      public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer( const Ref< VertexBuffer > &pVertexBuffer ) = 0;
        virtual void AddIndexBuffer( const Ref< IndexBuffer > &pIndexBuffer ) = 0;
        virtual const BufferLayout &GetLayout() const = 0;

        static VertexBuffer *Create();
    };
}