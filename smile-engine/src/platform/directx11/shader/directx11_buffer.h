#pragma once
#include "smile_engine/graphic/shader/buffer.h"

#include <d3d11.h>

namespace Smile::Graphic
{
    D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage );
}