/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "image.h"

namespace smile::graphic
{
    Image::Image() noexcept
        : m_Width{ 0 }, m_Height{ 0 }, m_Format{ rhi::Format::UNKNOWN }, m_Buffer{}, m_BytesPerPixel{ 0 }
    {
    }

    Image::Image( const Uint32 width, const Uint32 height, const rhi::Format format, const Byte *pData )
        : m_Width{ width }, m_Height{ height }, m_Format{ format }
    {
        const auto &formatInfo = GetFormatInfo( format );
        m_BytesPerPixel = formatInfo.BytesPerBlock;

        const Count size = [&]()
        {
            if ( m_BytesPerPixel == 0 )
                return m_Width * m_Height / 2;
            else
                return m_BytesPerPixel * m_Width * m_Height;
        }();

        m_Buffer.SetItemCount( size );
        memory::SetByteArray( m_Buffer.GetData(), pData, m_Buffer.GetItemCount() );
    }
}