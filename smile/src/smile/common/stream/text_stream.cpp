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
 * @file        text_stream.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stream for buffering and writing text
 */
#include "smpch.h"
#include "text_stream.h"

namespace smile::stream
{
    TextStream::TextStream() noexcept : CharStream{}
    {
    }

    TextStream::~TextStream()
    {
        Reset();
    }

    void TextStream::Reset()
    {
        CharStream::Reset();
        m_Text.SetCharCount( 0 );
    }

    void TextStream::Write( const char *buffer, const Count charCount )
    {
        m_Text += primitive::StringView{ buffer, charCount };
    }

    void TextStream::Rewind( const Index charIndex )
    {
        const Count currentCharCount = m_Text.GetCharCount() + m_CurrentCharIndex;

        SM_ASSERT( charIndex <= currentCharCount );

        if ( charIndex < m_Text.GetCharCount() )
        {
            m_Text.SetCharCount( charIndex );
            CharStream::Reset();
        }
        else
        {
            SetCurrentCharIndex( charIndex - m_Text.GetCharCount() );
        }
    }
}