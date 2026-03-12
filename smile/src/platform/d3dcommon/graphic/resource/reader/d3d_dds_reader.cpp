/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/graphic/resource/reader/dds_reader.h"

#include "smile/graphic/resource/image.h"
#include "platform/d3dcommon/graphic/d3d_diagnostics.h"

#include <DirectXTex.h>

namespace smile::graphic
{
    memory::Ref< Image > DDSReader::Read( const std::filesystem::path &path )
    {
        DirectX::TexMetadata info{};
        DirectX::ScratchImage image{};

        HRESULT result = DirectX::LoadFromDDSFile( path.c_str(), DirectX::DDS_FLAGS_NONE, &info, image );

        if ( FAILED( result ) )
        {
            SM_LOG_ERROR( "DDSReader::Read > Loading from DDS file failed: {}, returning nullptr",
                rhi::GetD3DErrorMessageString( result ) );

            return memory::CreateRef< Image >();
        }

        switch ( info.format )
        {
            case DXGI_FORMAT_R8G8B8A8_UNORM:
                return memory::CreateRef< Image >(
                    info.width, info.height, rhi::Format::RGBA8_UNORM, image.GetPixels() );

            case DXGI_FORMAT_BC1_UNORM:
                return memory::CreateRef< Image >( info.width, info.height, rhi::Format::BC1_UNORM, image.GetPixels() );

            default:
                SM_ASSERT_MSG( false, "DDSReader::Read > Unsupported image format" );
        }
    }
}