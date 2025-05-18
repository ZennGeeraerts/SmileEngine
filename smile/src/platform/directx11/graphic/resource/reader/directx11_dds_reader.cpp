/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "smile/graphic/resource/reader/dds_reader.h"

#include "smile/graphic/resource/image.h"
#include "platform/directx11/graphic/renderer_backend/directx11_diagnostics.h"

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
                fmt::ptr( GetDirectX11ErrorMessage( result ) ) );

            return memory::CreateRef< Image >();
        }

        switch ( info.format )
        {
            case DXGI_FORMAT_R8G8B8A8_UNORM:
                return memory::CreateRef< Image >( info.width, info.height, ImageFormat::RGBA, image.GetPixels() );

            case DXGI_FORMAT_BC1_UNORM:
                return memory::CreateRef< Image >( info.width, info.height, ImageFormat::BC1, image.GetPixels() );

            default:
                SM_ASSERT_MSG( false, "DDSReader::Read > Unsupported image format" );
        }
    }
}