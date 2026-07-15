/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/flags.h"
#include "smile/common/foundation/hash_code.h"
#include "smile/common/primitive/handle_manager.h"
#include "smile/graphic/rhi/cpu_access_mode.h"
#include "smile/graphic/rhi/format.h"

namespace smile::graphic::rhi
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
        [[nodiscard]] TextureSlice Resolve( const TextureDescriptor &desc ) const;

        Uint32 x = 0;
        Uint32 y = 0;
        Uint32 z = 0;

        Uint32 Width = std::numeric_limits< Uint32 >::max();
        Uint32 Height = std::numeric_limits< Uint32 >::max();
        Uint32 Depth = std::numeric_limits< Uint32 >::max();

        MipmapLevel MipLevelCount = 0;
        ArraySlice Slice = 0;
    };

    struct TextureSubresourceSet
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

        [[nodiscard]] TextureSubresourceSet Resolve( const TextureDescriptor &desc, bool useSingleMipLevel ) const;
        [[nodiscard]] bool IsEntireTexture( const TextureDescriptor &desc ) const;

        static constexpr MipmapLevel MaxMipLevels = std::numeric_limits< Uint32 >::max();
        static constexpr ArraySlice MaxArraySlices = std::numeric_limits< Uint32 >::max();

        MipmapLevel BaseMipLevel = 0;
        MipmapLevel MipLevelCount = 1;
        ArraySlice BaseArraySlice = 0;
        ArraySlice ArraySliceCount = 1;

        static constexpr MipmapLevel s_AllMipMapLevels = std::numeric_limits< MipmapLevel >::max();
        static constexpr ArraySlice s_AllSlices = std::numeric_limits< ArraySlice >::max();
    };

    static const TextureSubresourceSet s_AllSubresources{ 0,
        TextureSubresourceSet::s_AllMipMapLevels,
        0,
        TextureSubresourceSet::s_AllSlices };

    struct TextureBindingKey final : public TextureSubresourceSet
    {
        TextureBindingKey()
        {
        }

        TextureBindingKey( const TextureSubresourceSet &subresourceSet, Format format, bool isReadOnlyDSV = false )
            : TextureSubresourceSet{ subresourceSet }, TextureFormat{ format }, IsReadOnlyDSV{ isReadOnlyDSV }
        {
        }

        foundation::HashCode GetHashCode() const
        {
            foundation::HashCode hash = 0;
            hash = foundation::HashCombine( hash, std::hash< Format >{}( TextureFormat ) );
            hash = foundation::HashCombine( hash, std::hash< MipmapLevel >{}( BaseMipLevel ) );
            hash = foundation::HashCombine( hash, std::hash< MipmapLevel >{}( MipLevelCount ) );
            hash = foundation::HashCombine( hash, std::hash< ArraySlice >{}( BaseArraySlice ) );
            hash = foundation::HashCombine( hash, std::hash< ArraySlice >{}( ArraySliceCount ) );
            return hash;
        }

        bool operator==( const TextureBindingKey &other ) const
        {
            return TextureFormat == other.TextureFormat && BaseMipLevel == other.BaseMipLevel &&
                   MipLevelCount == other.MipLevelCount && BaseArraySlice == other.BaseArraySlice &&
                   ArraySliceCount == other.ArraySliceCount;
        }

        Format TextureFormat = Format::UNKNOWN;
        bool IsReadOnlyDSV = false;
    };

    using TextureHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using TextureHandle = TextureHandleManager::HandleType;

    using StagingTextureHandleManager = typename primitive::HandleManager< Uint64, 32, 32 >;
    using StagingTextureHandle = StagingTextureHandleManager::HandleType;

    static constexpr Uint16 s_MaxTextureCount = ( 4 << 10 );
}

namespace std
{
    template <>
    struct hash< smile::graphic::rhi::TextureBindingKey >
    {
        smile::foundation::HashCode operator()( const smile::graphic::rhi::TextureBindingKey &textureBindingKey ) const
        {
            return textureBindingKey.GetHashCode();
        }
    };
}