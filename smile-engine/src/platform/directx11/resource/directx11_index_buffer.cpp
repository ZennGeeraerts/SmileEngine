#include "smpch.h"
#include "directx11_index_buffer.h"

namespace smile::graphic
{
    DirectX11IndexBuffer::~DirectX11IndexBuffer()
    {
        SAFE_RELEASE( pInternal );
    }
}