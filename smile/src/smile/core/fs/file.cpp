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
 * @file        file.cpp
 * @author      Zenn Geeraerts
 * @created     28 November 2025
 * @brief       File IO operations
 */
#include "smpch.h"
#include "file.h"

namespace smile::fs
{
    File::File( const Path &path ) noexcept : m_Path{ path }
    {
    }

    bool File::SetIndex( const Index index )
    {
        m_FileStream.clear();
        m_FileStream.seekg( index );
        m_FileStream.seekp( index );

        if ( !m_FileStream )
        {
            return false;
        }

        UpdateIndex( index );
        return true;
    }

    bool File::Open( bool isInput, bool isOutput, stream::OpeningModeFlags flags )
    {
        auto mode = std::ios_base::binary | ( isInput ? std::ios_base::in : 0 ) | ( isOutput ? std::ios_base::out : 0 );

        if ( flags.Has( stream::OpeningMode::Truncate ) )
        {
            mode |= std::ios_base::trunc;
        }

        if ( flags.Has( stream::OpeningMode::Append ) )
        {
            mode |= std::ios_base::app;
        }

        m_FileStream.open( m_Path.GetData(), static_cast< std::ios_base::openmode >( mode ) );

        SetInput( isInput );
        SetOutput( isOutput );

        const bool isOpen = m_FileStream.is_open();

        SetOpen( isOpen );

        if ( isOpen )
        {
            m_FileStream.seekg( 0, std::ios_base::end );
            UpdateSize( foundation::NumericCast< Count >( m_FileStream.tellg() ) );
            m_FileStream.seekg( 0 );
            UpdateIndex( 0 );
            return true;
        }
        else
        {
            return false;
        }
    }

    bool File::OpenInput()
    {
        return Open( true, false, {} );
    }

    bool File::OpenOutput( stream::OpeningModeFlags openingModeFlags )
    {
        return Open( false, true, openingModeFlags );
    }

    bool File::OpenInputOutput( stream::OpeningModeFlags openingModeFlags )
    {
        return Open( true, true, openingModeFlags );
    }

    Count File::ReadByteArray( void *pByteArray, Count size )
    {
        SM_ASSERT( IsInput() );

        m_FileStream.read( reinterpret_cast< char * >( pByteArray ), size );

        const auto bytesRead = foundation::NumericCast< Count >( m_FileStream.bad() ? 0 : m_FileStream.gcount() );
        UpdateIndex( GetIndex() + bytesRead );

        return bytesRead;
    }

    Count File::WriteByteArray( const void *pByteArray, const Count size )
    {
        SM_ASSERT( IsOutput() );

        const auto currentIndex = GetIndex();

        m_FileStream.write( reinterpret_cast< const char * >( pByteArray ), size );

        auto currentWriteCursor = foundation::NumericCast< Count >( m_FileStream.tellp() );

        if ( currentWriteCursor > GetSize() )
        {
            UpdateSize( currentWriteCursor );
        }

        UpdateIndex( currentWriteCursor );

        return currentWriteCursor - currentIndex;
    }

    bool File::Close()
    {
        m_FileStream.close();
        SetOpen( false );

        return true;
    }
}