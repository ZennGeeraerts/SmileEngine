#include "smpch.h"
#include "texture.h"

namespace smile::graphic
{
    TextureSlice TextureSlice::Resolve( const TextureDescriptor &desc ) const
    {
        TextureSlice result{ *this };

        SM_ASSERT( MipLevelCount < desc.MipLevelCount,
            "TextureSlice::Resolve > Mipmap level needs to be smaller than the mipmap level of the texture "
            "descriptor" );

        if ( Width == std::numeric_limits< Uint32 >::max() )
            result.Width = std::max( desc.Width >> MipLevelCount, 1u );

        if ( Height == std::numeric_limits< Uint32 >::max() )
            result.Height = std::max( desc.Height >> MipLevelCount, 1u );

        if ( Depth == std::numeric_limits< Uint32 >::max() )
        {
            if ( desc.Dimension == TextureDimension::Texture3D )
                result.Depth = std::max( desc.Depth >> MipLevelCount, 1u );
            else
                result.Depth = 1;
        }

        return result;
    }

    TextureSubresourceSet TextureSubresourceSet::Resolve( const TextureDescriptor &desc, bool useSingleMipLevel ) const
    {
        TextureSubresourceSet result;
        result.BaseMipLevel = BaseMipLevel;

        if ( useSingleMipLevel )
        {
            result.MipLevelCount = 1;
        }
        else
        {
            int lastMipLevelPlusOne = std::min( BaseMipLevel + MipLevelCount, desc.MipLevelCount );
            result.MipLevelCount = MipmapLevel{ std::max( 0u, lastMipLevelPlusOne - BaseMipLevel ) };
        }

        switch ( desc.Dimension )
        {
            case TextureDimension::Texture1DArray:
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
            {
                result.BaseArraySlice = BaseArraySlice;
                int lastArraySlicePlusOne = std::min( BaseArraySlice + ArraySliceCount, desc.ArrayLength );
                result.ArraySliceCount = ArraySlice{ std::max( 0u, lastArraySlicePlusOne - BaseArraySlice ) };
                break;
            }
            default:
                result.BaseArraySlice = 0;
                result.ArraySliceCount = 1;
                break;
        }

        return result;
    }

    bool TextureSubresourceSet::IsEntireTexture( const TextureDescriptor &desc ) const
    {
        if ( BaseMipLevel > 0u || BaseMipLevel + MipLevelCount < desc.MipLevelCount )
            return false;

        switch ( desc.Dimension )
        {
            case TextureDimension::Texture1DArray:
            case TextureDimension::Texture2DArray:
            case TextureDimension::TextureCube:
            case TextureDimension::TextureCubeArray:
            {
                if ( BaseArraySlice > 0u || BaseArraySlice + ArraySliceCount < desc.ArrayLength )
                    return false;
                else
                    return true;
            }
            default:
                return true;
        }
    }
}