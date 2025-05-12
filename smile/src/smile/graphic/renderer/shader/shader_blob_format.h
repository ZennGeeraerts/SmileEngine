/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

#include <string>

namespace smile::graphic
{
    enum class ShaderBlobFormat : Uint16
    {
        Unknown,
        DXBC,
        DXIL,
        SPIRV
    };

    static ShaderBlobFormat ShaderBlobFormatFromString( const std::string &blobFormat )
    {
        if ( blobFormat == "dxbc" )
            return ShaderBlobFormat::DXBC;
        else if ( blobFormat == "dxil" )
            return ShaderBlobFormat::DXIL;
        else if ( blobFormat == "spirv" )
            return ShaderBlobFormat::SPIRV;

        return ShaderBlobFormat::Unknown;
    }
}