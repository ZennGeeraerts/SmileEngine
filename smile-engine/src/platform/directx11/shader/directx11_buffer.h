#pragma once
#include "smile_engine/renderer/shader/buffer.h"

#include <d3d11.h>

namespace smile::renderer
{
    D3D11_USAGE bufferUsageToDirectXType( BufferUsage buffer_usage );
}