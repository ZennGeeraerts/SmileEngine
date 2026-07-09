#pragma once

#include "smile/graphic/renderer/resource/vertex_buffer.h"
#include "smile/graphic/renderer/resource/index_buffer.h"

namespace smile::graphic
{
    struct SpriteData final
    {
        VertexBuffer QuadVertexBuffer;
        IndexBuffer QuadIndexBuffer;
        rhi::BufferLayout VertexLayout;
    };
}
