/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "image.h"

namespace smile::graphic
{
    Image::Image() : m_Width{ 0 }, m_Height{ 0 }, m_Format{ ImageFormat::None }, m_Buffer{}, m_Stride{ 0 }
    {
    }

    Image::Image( const Uint32 width, const Uint32 height, const ImageFormat format, const Byte *pData )
        : m_Width{ width }, m_Height{ height }, m_Format{ format }
    {
        switch ( format )
        {
            case ImageFormat::RGBA:
            case ImageFormat::BGRA:
            {
                m_Stride = 4;
            }
            break;

            case ImageFormat::RGB:
            {
                m_Stride = 3;
            }
            break;

            case ImageFormat::BC1:
            case ImageFormat::BC4:
            {
                m_Stride = 0;
            }
            break;

            case ImageFormat::RG32F:
            {
                m_Stride = 8;
            }
            break;

            default:
            {
                m_Stride = 0;
                SM_ASSERT( false, "Image > Unsupported format" )
            }
            break;
        }

        const Uint32 size = [&]()
        {
            if ( m_Stride == 0 )
                return m_Width * m_Height / 2;
            else
                return m_Stride * m_Width * m_Height;
        }();

        m_Buffer.resize( size );
        std::memcpy( m_Buffer.data(), pData, size );
    }
}