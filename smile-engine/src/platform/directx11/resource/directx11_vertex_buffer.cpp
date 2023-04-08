#include "smpch.h"
#include "directX11_vertex_buffer.h"

namespace smile::graphic
{
    DirectX11VertexBuffer::~DirectX11VertexBuffer()
    {
        SAFE_RELEASE( pInternal );
    }
}