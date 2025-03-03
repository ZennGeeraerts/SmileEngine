/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "engine/graphic/renderer_backend/resource/vertex_buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11VertexBuffer final : public VertexBuffer
    {
        virtual ~DirectX11VertexBuffer();

        void *GetInternal() const override
        {
            return pInternal;
        }

        ID3D11Buffer *pInternal;
    };
}