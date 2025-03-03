/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_backend/shader/buffer.h"
#include "memory/object.h"

namespace smile::graphic
{
    struct IndexBufferDescriptor final
    {
        Uint32 *pIndices = nullptr;
        Uint32 Count = 0;
        BufferUsage Usage = BufferUsage::Default;
    };

    struct IndexBuffer : public memory::Object
    {
        IndexBuffer() = default;
        virtual ~IndexBuffer() = default;
        virtual void *GetInternal() const = 0;

        Uint32 Count;
    };
}