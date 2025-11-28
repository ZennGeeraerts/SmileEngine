/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "memory_binary_stream.h"

namespace smile::stream
{
    bool MemoryBinaryStream::SetIndex( const Index index )
    {
        UpdateIndex( index );
        return true;
    }

    bool MemoryBinaryStream::OpenInput()
    {
        SetInput( true );
        SetOpen( true );

        UpdateIndex( 0 );
        UpdateSize( m_ByteArray.GetItemCount() );

        return true;
    }

    bool MemoryBinaryStream::OpenOutput( OpeningModeFlags openingModeFlags )
    {
        SetOutput( true );
        SetOpen( true );

        if ( openingModeFlags.Has( OpeningMode::Truncate ) )
        {
            m_ByteArray.SetItemCount( 0 );
        }

        const Count size = m_ByteArray.GetItemCount();
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
            m_ByteArray.SetItemCount( 0 );
        }

        const Count size = m_ByteArray.GetItemCount();
        UpdateSize( size );
        UpdateIndex( openingModeFlags.Has( OpeningMode::Append ) ? size : 0 );

        return true;
    }

    Count MemoryBinaryStream::ReadByteArray( void *pByteArray, Count size )
    {
        const auto index = GetIndex();
        const auto totalSize = GetSize();

        if ( index + size > totalSize )
        {
            size = totalSize - index;
        }

        std::memcpy( pByteArray, m_ByteArray.GetData() + index, size );

        UpdateIndex( index + size );

        return size;
    }

    Count MemoryBinaryStream::WriteByteArray( const void *pByteArray, const Count size )
    {
        const auto index = GetIndex();
        const auto totalSize = GetSize();

        if ( index + size > totalSize )
        {
            m_ByteArray.SetItemCount( index + size );
            UpdateSize( index + size );
        }

        std::memcpy( m_ByteArray.GetData() + index, pByteArray, size );
        UpdateIndex( index + size );

        return size;
    }

    bool MemoryBinaryStream::Close()
    {
        SetOpen( false );

        return true;
    }
}