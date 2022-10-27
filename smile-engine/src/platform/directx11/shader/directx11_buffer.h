#pragma once
#include "smile_engine/graphic/shader/buffer.h"

#include <d3d11.h>

namespace smile::graphic
{
    D3D11_USAGE bufferUsageToDirectXType( BufferUsage buffer_usage );
}