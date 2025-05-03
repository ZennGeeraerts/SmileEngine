/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "memory_binary_stream.h"

namespace smile::stream
{
    bool MemoryBinaryStream::SetIndex( const Uint32 index )
    {
        UpdateIndex( index );
        return true;
    }

    bool MemoryBinaryStream::OpenInput()
    {
        SetInput( true );
        SetOpen( true );

        UpdateIndex( 0 );
        UpdateSize( static_cast< Uint32 >( m_ByteArray.size() ) );

        return true;
    }

    bool MemoryBinaryStream::OpenOutput( OpeningModeFlags openingModeFlags )
    {
        SetOutput( true );
        SetOpen( true );

        if ( openingModeFlags.Has( OpeningMode::Truncate ) )
        {
            m_ByteArray.resize( 0 );
        }

        Uint32 size = static_cast< Uint32 >( m_ByteArray.size() );
        UpdateSize( size );
        UpdateIndex( openingModeFlags.Has( OpeningMode::Append ) ? size : 0 );

        return true;
    }

    bool MemoryBinaryStream::OpenInputOutput( OpeningModeFlags openingModeFlags )
    {
        SetInput( true );
        SetOutput( true );
        SetOpen( true );

        if ( openingModeFlags.Has( OpeningMode::Truncate ) )
        {
            m_ByteArray.resize( 0 );
        }

        Uint32 size = static_cast< Uint32 >( m_ByteArray.size() );
        UpdateSize( size );
        UpdateIndex( openingModeFlags.Has( OpeningMode::Append ) ? size : 0 );

        return true;
    }

    Uint32 MemoryBinaryStream::ReadByteArray( void *pByteArray, Uint32 size )
    {
        const auto index = GetIndex();
        const auto totalSize = GetSize();

        if ( index + size > totalSize )
        {
            size = totalSize - index;
        }

        std::memcpy( pByteArray, m_ByteArray.data() + index, size );

        UpdateIndex( index + size );

        return size;
    }

    Uint32 MemoryBinaryStream::WriteByteArray( const void *pByteArray, const Uint32 size )
    {
        const auto index = GetIndex();
        const auto totalSize = GetSize();

        if ( index + size > totalSize )
        {
            m_ByteArray.resize( index + size );
            UpdateSize( index + size );
        }

        std::memcpy( m_ByteArray.data() + index, pByteArray, size );
        UpdateIndex( index + size );

        return size;
    }

    bool MemoryBinaryStream::Close()
    {
        SetOpen( false );

        return true;
    }
}