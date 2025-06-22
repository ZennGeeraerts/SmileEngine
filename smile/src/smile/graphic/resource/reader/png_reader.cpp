/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "png_reader.h"

#include "smile/graphic/resource/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace smile::graphic
{
    memory::Ref< Image > PNGReader::Read( const std::filesystem::path &path )
    {
        int width;
        int height;
        int channelsPerPixel;

        std::filesystem::path finalPath = [&]()
        {
            if ( path.is_absolute() )
                return path;
            else
                return std::filesystem::absolute( path );
        }();

        SM_ASSERT_MSG(
            std::filesystem::exists( finalPath ), "PNGReader::Read > Path: {} does not exist", finalPath.string() );

        stbi_uc *pData = stbi_load( finalPath.string().c_str(), &width, &height, &channelsPerPixel, 4 );

        if ( !pData )
            return memory::CreateRef< Image >();

        memory::Ref< Image > pImage = [&]()
        {
            switch ( channelsPerPixel )
            {
                case 4:
                    return memory::CreateRef< Image >( width, height, Format::RGBA8_UNORM, pData );

                default:
                    SM_ASSERT_MSG( false, "PNGReader::Read > Unsupported channels per pixel" );
            }
        }();

        stbi_image_free( pData );

        return pImage;
    }
}