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
 * @file        char_stream.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stream for buffering and writing characters
 */
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::stream
{
    class CharStream
    {
      public:
        CharStream() noexcept : m_CurrentCharIndex{ 0 }
        {
            m_Buffer[m_CurrentCharIndex] = 0;
        }

        CharStream( const CharStream & ) = delete;
        CharStream( CharStream && ) = delete;

        virtual ~CharStream()
        {
            SM_ASSERT( m_CurrentCharIndex == 0 ); // Make sure flush is called before destruction
        }

        CharStream &operator=( const CharStream & ) = delete;
        CharStream &operator=( CharStream && ) = delete;

        bool IsEmpty() const
        {
            return m_CurrentCharIndex == 0;
        }

        void Reset()
        {
            m_CurrentCharIndex = 0;
            m_Buffer[m_CurrentCharIndex] = 0;
        }

        void AddString( const char *text );
        void AddString( const char *text, const Count charCount );

        void Flush()
        {
            m_Buffer[m_CurrentCharIndex] = 0;
            Write( m_Buffer, m_CurrentCharIndex );
            m_CurrentCharIndex = 0;
        }

      protected:
        virtual void Write( const char *text, const Count charCount ) = 0;
        virtual void Rewind( const Index charIndex ) = 0;

        const char *GetData()
        {
            m_Buffer[m_CurrentCharIndex] = 0;

            return m_Buffer;
        }

        void SetCurrentCharIndex( const Index charIndex )
        {
            m_CurrentCharIndex = charIndex;
            m_Buffer[m_CurrentCharIndex] = 0;
        }

        Index m_CurrentCharIndex;

      private:
        constexpr static Count s_BufferSize{ 1023 }; // Reserve 1 byte for null termination

        char m_Buffer[s_BufferSize];
    };

    inline CharStream &operator<<( CharStream &stream, const char *value )
    {
        stream.AddString( value );

        return stream;
    }

    inline CharStream &operator<<( CharStream &stream, const char value )
    {
        stream.AddString( &value, 1 );

        return stream;
    }

    CharStream &operator<<( CharStream &stream, const primitive::StringView value );
    CharStream &operator<<( CharStream &stream, const void *ptr );
    CharStream &operator<<( CharStream &stream, short value );
    CharStream &operator<<( CharStream &stream, unsigned short value );
    CharStream &operator<<( CharStream &stream, int value );
    CharStream &operator<<( CharStream &stream, unsigned int value );
    CharStream &operator<<( CharStream &stream, long value );
    CharStream &operator<<( CharStream &stream, unsigned long value );
    CharStream &operator<<( CharStream &stream, long long value );
    CharStream &operator<<( CharStream &stream, unsigned long long value );
    CharStream &operator<<( CharStream &stream, float value );
    CharStream &operator<<( CharStream &stream, double value );
    CharStream &operator<<( CharStream &stream, bool value );
}