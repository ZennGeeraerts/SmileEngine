#pragma once
#include "smile_engine/graphic/resource/index_buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    struct DirectX11IndexBuffer final : public IndexBuffer
    {
        DirectX11IndexBuffer() = default;
        virtual ~DirectX11IndexBuffer();

        void *GetInternal() const override
        {
            return pInternal;
        }

        ID3D11Buffer *pInternal;
    };
}
