/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"

namespace smile::graphic
{
    enum class ShaderBlobFormat : Uint16
    {
        Unknown,
        DXBC,
        DXIL,
        SPIRV
    };

    static ShaderBlobFormat ShaderBlobFormatFromString( const primitive::StringView blobFormat )
    {
        if ( blobFormat == "dxbc" )
            return ShaderBlobFormat::DXBC;
        else if ( blobFormat == "dxil" )
            return ShaderBlobFormat::DXIL;
        else if ( blobFormat == "spirv" )
            return ShaderBlobFormat::SPIRV;

        return ShaderBlobFormat::Unknown;
    }

    static primitive::String ShaderBlobFormatToString( ShaderBlobFormat blobFormat )
    {
        switch ( blobFormat )
        {
            case ShaderBlobFormat::DXBC:
                return "dxbc";
            case ShaderBlobFormat::DXIL:
                return "dxil";
            case ShaderBlobFormat::SPIRV:
                return "spirv";
        }

        return "";
    }
}