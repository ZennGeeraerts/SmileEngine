/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_backend/shader/buffer.h"
#include "memory/object.h"

namespace smile::graphic
{
    struct VertexBufferDescriptor final
    {
        void *pVertices = nullptr;
        Uint32 Count = 0;
        Uint32 Stride = 0;
        BufferUsage Usage = BufferUsage::Default;
        BufferCPUAccess CPUAccess = BufferCPUAccess::None;
    };

    struct VertexBuffer : public memory::Object
    {
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;
        virtual void *GetInternal() const = 0;

        Uint32 Stride = 0;
    };
}