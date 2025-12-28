/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "binary_stream.h"

namespace smile::stream
{
    BinaryStream::BinaryStream()
        : memory::Counted{}, m_Size{ 0 }, m_Index{ 0 }, m_IsOpen{ false }, m_IsInput{ false }, m_IsOutput{ false }
    {
    }

    BinaryStream::~BinaryStream()
    {
        SM_ASSERT_MSG( !IsOpen(), "Binary stream is open while being destructed" );
    }

    void GetTextContent( primitive::String &content, BinaryStream &stream )
    {
        if ( stream.GetSize() == 0 )
        {
            return;
        }

        content.SetCharCount( stream.GetSize() );

        stream.ReadByteArray( content.GetData(), stream.GetSize() );

        for ( const auto index : foundation::GetReverseCountIterator( content.GetCharCount() ) )
        {
            if ( content[index] == '\r' )
            {
                content.EraseCharAtIndex( index );
            }
        }
    }
}