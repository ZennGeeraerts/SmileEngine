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
        int stridePerPixel;

        std::filesystem::path finalPath = [&]()
        {
            if ( path.is_absolute() )
                return path;
            else
                return std::filesystem::absolute( path );
        }();

        SM_ASSERT(
            std::filesystem::exists( finalPath ), "PNGReader::Read > Path: {} does not exist", finalPath.string() );

        stbi_uc *pData = stbi_load( finalPath.string().c_str(), &width, &height, &stridePerPixel, 0 );

        if ( pData )
            return memory::CreateRef< Image >();

        memory::Ref< Image > pImage = [&]()
        {
            switch ( stridePerPixel )
            {
                case 4:
                    return memory::CreateRef< Image >( width, height, ImageFormat::RGBA, pData );
                case 3:
                    return memory::CreateRef< Image >( width, height, ImageFormat::RGB, pData );

                default:
                    SM_ASSERT( false, "PNGReader::Read > Unsupported stride per pixel" );
            }
        }();

        stbi_image_free( pData );

        return pImage;
    }
}