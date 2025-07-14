/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "directx11_texture.h"

namespace smile::graphic::rhi
{
    class DirectX11Context;

    struct DirectX11StagingTexture final
    {
        DirectX11StagingTexture() = default;
        ~DirectX11StagingTexture() = default;

        DirectX11StagingTexture( const DirectX11StagingTexture & ) = delete;
        DirectX11StagingTexture( DirectX11StagingTexture && ) = delete;
        DirectX11StagingTexture &operator=( const DirectX11StagingTexture & ) = delete;
        DirectX11StagingTexture &operator=( DirectX11StagingTexture && ) = delete;

        void Create( DirectX11Context &context, const TextureDescriptor &desc );
        void Destroy();

        void *Map( DirectX11Context &context, const TextureSlice &slice, CPUAccessMode cpuAccess );
        void Unmap( DirectX11Context &context );

        DirectX11Texture Texture{};
        CPUAccessMode CPUAccess = CPUAccessMode::None;
        Uint32 MappedSubresource = std::numeric_limits< Uint32 >::max();
    };
}