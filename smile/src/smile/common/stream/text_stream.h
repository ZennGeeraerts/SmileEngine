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
 * @file        text_stream.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stream for buffering and writing text
 */
#pragma once

#include "char_stream.h"
#include "smile/common/primitive/text/string.h"

namespace smile::stream
{
    class TextStream final : public CharStream
    {
      public:
        TextStream() noexcept;
        TextStream( const TextStream & ) = delete;
        TextStream( TextStream && ) = delete;
        ~TextStream() override;

        TextStream &operator=( const TextStream & ) = delete;
        TextStream &operator=( TextStream && ) = delete;

        primitive::StringView GetText()
        {
            if ( m_Text.IsEmpty() )
            {
                return { GetData(), m_CurrentCharIndex };
            }
            else
            {
                Flush();

                return m_Text;
            }
        }

        void Reset();
        void Write( const char *buffer, const Count charCount ) override;
        void Rewind( const Index charIndex ) override;

      private:
        primitive::String m_Text;
    };

    inline CharStream &operator<<( CharStream &charStream, TextStream &textStream )
    {
        charStream << textStream.GetText();

        return charStream;
    }
}