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
 * @file        char_stream.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stream for buffering and writing characters
 */
#include "smpch.h"
#include "char_stream.h"

namespace smile::stream
{
    void CharStream::AddString( const char *text )
    {
        AddString( text, foundation::NumericCast< Count >( strlen( text ) ) );
    }

    void CharStream::AddString( const char *text, const Count charCount )
    {
        Count remainingCharCount{ charCount };

        do
        {
            const Count copiedSize = std::min( remainingCharCount, s_BufferSize - m_CurrentCharIndex );

            std::memcpy( m_Buffer + m_CurrentCharIndex, text + charCount - remainingCharCount, copiedSize );
            m_CurrentCharIndex += copiedSize;
            remainingCharCount -= copiedSize;

            if ( m_CurrentCharIndex + remainingCharCount > s_BufferSize )
            {
                Flush();
            }

        } while ( remainingCharCount > 0 );
    }

    CharStream &operator<<( CharStream &stream, const primitive::StringView value )
    {
        stream.AddString( value.GetSubText(), value.GetCharCount() );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, const void *ptr )
    {
        char temp[64];

        std::snprintf( temp, 64, "%llx", reinterpret_cast< UintPointer >( ptr ) );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, short value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%hd", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, unsigned short value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%hu", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, int value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%i", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, unsigned int value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%u", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, long value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%l", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, unsigned long value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%lu", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, long long value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%lld", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, unsigned long long value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%llu", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, float value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%f", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, double value )
    {
        char temp[64];

        std::snprintf( temp, 64, "%f", value );
        stream.AddString( temp );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, bool value )
    {
        stream.AddString( value ? "true" : "false" );

        return stream;
    }
}