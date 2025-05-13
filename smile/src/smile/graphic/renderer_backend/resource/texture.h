/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/flags.h"
#include "smile/graphic/renderer_backend/cpu_access_mode.h"
#include "smile/graphic/renderer_backend/format.h"

namespace smile::graphic
{
    using MipmapLevel = Uint32;
    using ArraySlice = Uint32;

    enum class TextureDimension : Uint8
    {
        Unknown,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        TextureCube,
        TextureCubeArray,
        Texture3D
    };

    enum class TextureBindFlags : Uint8
    {
        None,
        ShaderResource,
        UnorderedAccess,
        RenderTarget
    };

    struct TextureDescriptor final
    {
        Uint32 Width = 1;
        Uint32 Height = 1;
        Uint32 Depth = 1;
        Uint32 ArrayLength = 1;
        Uint32 MipLevelCount = 1;
        Uint32 SampleCount = 1;
        Uint32 SampleQuality = 0;
        Format TextureFormat = Format::UNKNOWN;
        TextureDimension Dimension = TextureDimension::Texture2D;
        CPUAccessMode CPUAccess = CPUAccessMode::None;
        foundation::Flags< TextureBindFlags > BindFlags{ TextureBindFlags::None };
        bool IsTypeless = false;
    };

    struct TextureSlice final
    {
        Uint32 x = 0;
        Uint32 y = 0;
        Uint32 z = 0;

        Uint32 Width = std::numeric_limits< Uint32 >::max();
        Uint32 Height = std::numeric_limits< Uint32 >::max();
        Uint32 Depth = std::numeric_limits< Uint32 >::max();

        MipmapLevel MipLevelCount = 0;
        ArraySlice Slice = 0;

        [[nodiscard]] TextureSlice Resolve( const TextureDescriptor &desc ) const;
    };

    struct TextureSubresourceSet final
    {
        TextureSubresourceSet() = default;

        TextureSubresourceSet( MipmapLevel baseMipLevel,
            MipmapLevel mipLevelCount,
            ArraySlice baseArraySlice,
            ArraySlice arraySliceCount )
            : BaseMipLevel{ baseMipLevel },
              MipLevelCount{ mipLevelCount },
              BaseArraySlice{ baseArraySlice },
              ArraySliceCount{ arraySliceCount }
        {
        }

        static constexpr MipmapLevel MaxMipLevels = std::numeric_limits< Uint32 >::max();
        static constexpr ArraySlice MaxArraySlices = std::numeric_limits< Uint32 >::max();

        MipmapLevel BaseMipLevel = 0;
        MipmapLevel MipLevelCount = 1;
        ArraySlice BaseArraySlice = 0;
        ArraySlice ArraySliceCount = 1;

        [[nodiscard]] TextureSubresourceSet Resolve( const TextureDescriptor &desc, bool useSingleMipLevel ) const;
        [[nodiscard]] bool IsEntireTexture( const TextureDescriptor &desc ) const;
    };
}