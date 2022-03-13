#pragma once
#include "smile_engine/renderer/shader/buffer.h"

#include <d3d11.h>

namespace smile
{
    D3D11_USAGE BufferUsageToDirectXType( BufferUsage bufferUsage );
}