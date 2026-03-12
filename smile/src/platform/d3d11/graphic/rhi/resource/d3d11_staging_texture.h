/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "d3d11_texture.h"

namespace smile::graphic::rhi
{
    class D3D11Context;

    struct D3D11StagingTexture final
    {
        D3D11StagingTexture() = default;
        ~D3D11StagingTexture();

        D3D11StagingTexture( const D3D11StagingTexture & ) = delete;
        D3D11StagingTexture( D3D11StagingTexture && ) = delete;
        D3D11StagingTexture &operator=( const D3D11StagingTexture & ) = delete;
        D3D11StagingTexture &operator=( D3D11StagingTexture && ) = delete;

        void Create( D3D11Context &context, const TextureDescriptor &desc );
        void Destroy();

        void *Map( D3D11Context &context, const TextureSlice &slice, CPUAccessMode cpuAccess );
        void Unmap( D3D11Context &context );

        D3D11Texture Texture{};
        CPUAccessMode CPUAccess = CPUAccessMode::None;
        Uint32 MappedSubresource = std::numeric_limits< Uint32 >::max();
    };
}