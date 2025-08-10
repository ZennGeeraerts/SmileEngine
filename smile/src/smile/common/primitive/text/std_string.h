/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "string.h"
#include "smile/common/memory/memory.h"

#include <string>

namespace smile::primitive
{
    class StdString final : public String
    {
      public:
        using value_type = char;
        static constexpr std::size_t npos = -1;

        StdString() = default;
        ~StdString() = default;

        StdString( const StdString & ) = default;
        StdString( StdString && ) = default;
        StdString &operator=( const StdString & ) = default;
        StdString &operator=( StdString && ) = default;

        StdString( const char *text ) : String{ text }
        {
        }

        StdString( const std::string &text ) : String{ text.c_str(), static_cast< Count >( text.size() ) }
        {
        }

        using String::operator[];

        StdString &operator=( const char *text )
        {
            String::operator=( text );

            return *this;
        }

        const char *data() const
        {
            return GetData();
        }

        const char *c_str() const
        {
            return GetData();
        }

        char back() const
        {
            return ( *this )[GetCharCount() - 1];
        }

        std::size_t size() const
        {
            return static_cast< std::size_t >( GetCharCount() );
        }

        void resize( Count size, char value )
        {
            const Count oldSize = GetCharCount();

            if ( oldSize < size )
            {
                SetCharCount( size );

                memory::FillByteArray( GetData() + oldSize, value, size - oldSize );
            }
        }

        void push_back( char c )
        {
            ( *this ) += c;
        }

        void append( const primitive::StringView text )
        {
            ( *this ) += text;
        }

        void append( const char *text, const Count charCount )
        {
            ( *this ) += primitive::StringView{ text, charCount };
        }

        void clear()
        {
            Clear();
        }
    };
}