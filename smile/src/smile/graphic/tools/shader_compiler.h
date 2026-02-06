/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        shader_compiler.h
 * @author      Zenn Geeraerts
 * @created     19 November 2025
 * @brief       Shader compiler tool
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/text/string.h"
#include "smile/graphic/shader/shader_blob_format.h"

namespace smile::graphic
{
    struct ShaderCompileOptions final
    {
        primitive::String EntryPoint;
        primitive::String TargetProfile;
        ShaderBlobFormat OutputFormat;
        bool Debug = false;
        bool Optimize = true;
    };

    class ShaderCompiler final
    {
      public:
        static bool Compile( const primitive::StringView source,
            const primitive::String &sourceName,
            const ShaderCompileOptions &options,
            primitive::Vector< Byte > &byteCode );
    };
}