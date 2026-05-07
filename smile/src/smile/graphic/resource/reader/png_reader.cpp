/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "png_reader.h"

#include "smile/core/fs/physical_system.h"
#include "smile/graphic/resource/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace smile::graphic
{
    memory::Ref< Image > PNGReader::Read( const fs::Path &path )
    {
        int width;
        int height;
        int channelsPerPixel;

        fs::Path finalPath = [&]()
        {
            if ( path.IsPhysical() )
                return path;
            else
                return fs::PhysicalSystem::GetAbsolutePath( path );
        }();

        SM_ASSERT_MSG( fs::PhysicalSystem::DoesFileExist( finalPath ),
            "PNGReader::Read > Path: {} does not exist",
            finalPath.string() );

        stbi_uc *pData = stbi_load( finalPath.GetData(), &width, &height, &channelsPerPixel, 4 );

        if ( !pData )
            return memory::CreateRef< Image >();

        memory::Ref< Image > pImage = [&]()
        {
            switch ( channelsPerPixel )
            {
                case 4:
                    return memory::CreateRef< Image >( width, height, rhi::Format::RGBA8_UNORM, pData );

                default:
                    SM_ASSERT_MSG( false, "PNGReader::Read > Unsupported channels per pixel" );
            }
        }();

        stbi_image_free( pData );

        return pImage;
    }
}