#pragma once
#include "smile_engine/graphic/resource/vertex_buffer.h"

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