/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/fixed_vector.h"
#include "render_handle.h"

namespace smile::graphic
{
    static constexpr Uint16 s_MaxBufferCount = ( 12 << 10 );
    static constexpr Uint16 s_MaxTextureCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxSamplerCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxFramebufferCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxBindingSetCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxShaderCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxGraphicsPipelineCount = ( 4 << 10 );
    static constexpr Uint16 s_MaxBindingLayoutCount = 5;
    static constexpr Uint16 s_MaxVertexAttributeCount = 16;
    static constexpr Uint16 s_MaxRenderTargets = 8;

    static constexpr Uint32 s_ConstantBufferOffsetSizeAlignment = 256;

    using BindingSetVector = primitive::FixedVector< BindingSetHandle, s_MaxBindingLayoutCount >;
}