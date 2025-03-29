/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "image_reader.h"

#include "smile/graphic/resource/image.h"
#include "png_reader.h"

namespace smile::graphic
{
    memory::Ref< Image > ImageReader::Read( const std::filesystem::path &path )
    {
        auto extension = path.extension();

        if ( extension == ".png" )
        {
            return PNGReader::Read( path );
        }

        SM_LOG_WARNING( "ImageReader::Read > Unknown extention: {}, returning nullptr", extension.string() );

        return nullptr;
    }
}