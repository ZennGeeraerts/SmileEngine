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
 * @file        path.h
 * @author      Zenn Geeraerts
 * @created     25 November 2025
 * @brief       File path class implementation
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/text/string.h"

namespace smile::fs
{
    class Path final : public primitive::String
    {
      public:
        Path() = default;

        Path( const primitive::StringView path ) : primitive::String{ path }
        {
        }

        Path( const char *path ) : primitive::String{ path }
        {
        }

        Path( const primitive::String &path ) : primitive::String{ path }
        {
        }

        Path( primitive::String &&path ) : primitive::String{ std::move( path ) }
        {
        }

        Path( const Path & ) = default;
        Path( Path && ) = default;

        ~Path() = default;

        Path &operator=( const Path & ) = default;
        Path &operator=( Path && ) = default;

        Path &operator=( const primitive::StringView text )
        {
            primitive::String::operator=( text );

            return *this;
        }

        Path &operator=( const primitive::String &text )
        {
            primitive::String::operator=( text );

            return *this;
        }

        Path &operator=( const char *text )
        {
            primitive::String::operator=( text );

            return *this;
        }

        primitive::StringView GetStem() const
        {
            return GetStem( *this );
        }

        primitive::StringView GetExtension() const
        {
            return GetExtension( *this );
        }

        primitive::StringView GetDirectory() const
        {
            return GetDirectory( *this );
        }

        primitive::StringView GetFileName() const
        {
            return GetFileName( *this );
        }

        void CanonicalizePath();

        static Count GetDirectoryCharacterCount( const primitive::StringView filePath );
        static Count GetExtensionCharacterCount( const primitive::StringView filePath );

        static primitive::StringView GetStem( const primitive::StringView filePath );
        static primitive::StringView GetExtension( const primitive::StringView filePath );
        static primitive::StringView GetDirectory( const primitive::StringView filePath );
        static primitive::StringView GetFileName( const primitive::StringView filePath );

        static Path FromPlatformPath( const primitive::StringView path );
    };
}

namespace std
{
    template <>
    struct formatter< smile::fs::Path >
    {
        formatter< basic_string_view< char > > Base;

        constexpr auto parse( format_parse_context &ctx )
        {
            return Base.parse( ctx );
        }

        auto format( const smile::fs::Path &path, format_context &ctx ) const
        {
            basic_string_view< char > temp{ path.GetData(), static_cast< size_t >( path.GetCharCount() ) };
            return Base.format( temp, ctx );
        }
    };
}