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
 * @file        main.cpp
 * @author      Zenn Geeraerts
 * @created     18 November 2025
 * @brief       Entry point for x-shader-c
 */
#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/array_view.h"
#include "smile/common/logging/logger.h"
#include "smile/core/commandline/option.h"
#include "smile/core/commandline/run.h"
#include "smile/core/fs/path.h"
#include "smile/core/fs/file.h"
#include "smile/graphic/tools/shader_compiler.h"
#include "smile/graphic/tools/shader_reflector.h"
#include "smile/graphic/shader/shader_serializer.h"

namespace
{
    using namespace smile;
    using namespace smile::commandline;

    Option< fs::Path > g_InputFilePath{ "--input", Description{ "Input shader source file" }, Short{ 'i' } };
    Option< fs::Path > g_OutputFilePath{ "--output",
        Description{ "Output compiled and reflected shader file" },
        Short{ 'o' } };
}

int main( int argc, const char **argv )
{
    if ( !commandline::Parse( { argv, foundation::NumericCast< Count >( argc ) } ) )
    {
        SM_LOG_ERROR( "Usage: x-shader-c -i <input_file> -o <output_file>" );
        return 1;
    }

    primitive::String entryPoint;
    primitive::String targetProfile;

    primitive::StringView fileName = ( *g_InputFilePath ).GetFileName();
    primitive::StringView stem = ( *g_InputFilePath ).GetStem();
    graphic::ShaderCompileOptions options{};
    options.OutputFormat = graphic::ShaderBlobFormat::DXBC;

    if ( fileName.Find( ".vs." ) != s_InvalidIndex )
    {
        options.EntryPoint = "VSMain";
        options.TargetProfile = "vs_5_0";
    }
    else if ( fileName.Find( ".ps." ) != s_InvalidIndex )
    {
        options.EntryPoint = "PSMain";
        options.TargetProfile = "ps_5_0";
    }
    else
    {
        SM_LOG_ERROR(
            "Could not detect whether a vertex shader or pixel shader was used: {0}", primitive::String{ fileName } );
        return 1;
    }

    primitive::String source{};
    fs::File file{ g_InputFilePath };
    file.OpenInput();
    source.SetCharCount( file.GetSize() );
    file.ReadByteArray( source.GetData(), file.GetSize() );
    file.Close();

    primitive::Vector< Byte > byteCode;
    if ( !graphic::ShaderCompiler::Compile( source.AsStringView(), stem, options, byteCode ) )
        return 2;

    graphic::ShaderReflectionData reflectionData;
    reflectionData.EntryPoint = options.EntryPoint;
    reflectionData.TargetProfile = options.TargetProfile;
    reflectionData.BlobFormat = options.OutputFormat;

    if ( !graphic::ShaderReflector::Reflect( options.OutputFormat, byteCode, reflectionData ) )
        return 2;

    const auto pShaderAsset = memory::CreateRef< graphic::ShaderAsset >( fileName, byteCode, reflectionData );
    graphic::ShaderSerializer serializer{ pShaderAsset };
    serializer.Serialize( ( *g_OutputFilePath ) );

    return 0;
}