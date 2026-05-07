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
 * @file        path.cpp
 * @author      Zenn Geeraerts
 * @created     25 November 2025
 * @brief       File path class implementation
 */
#include "smpch.h"
#include "path.h"

#include "smile/common/foundation/range_iterator.h"
#include "smile/common/primitive/text/utils.h"
#include "smile/common/stream/text_stream.h"

namespace smile::fs
{
    void Path::CanonicalizePath()
    {
        if ( IsEmpty() )
        {
            return;
        }

        primitive::Vector< primitive::StringView > segments;
        primitive::Split( segments, *this, '/' );

        if ( GetData()[0] == '/' )
        {
            segments.PushFront( primitive::StringView{} );
        }

        bool isDirty = false;

        for ( Index index{ 0 }; index < segments.GetItemCount(); ++index )
        {
            if ( segments[index] == ".." && index > 0 )
            {
                const auto first = segments.begin() + index - 1;
                const auto last = first + 2;
                segments.Erase( first, last );

                isDirty = true;
                index -= 2;
            }
            else if ( segments[index] == "." && index > 0 )
            {
                segments.EraseAtIndex( index );
                isDirty = true;
                --index;
            }
        }

        if ( isDirty )
        {
            if ( !segments.IsEmpty() )
            {
                stream::TextStream stream;
                stream << segments[0];

                for ( Index index = 1; index <= segments.GetLastIndex(); ++index )
                {
                    stream << '/' << segments[index];
                }

                operator=( stream.GetText() );
            }
            else
            {
                Clear();
            }
        }
    }

    Count Path::GetDirectoryCharacterCount( const primitive::StringView filePath )
    {
        for ( const auto charIndex : foundation::GetReverseCountIterator( filePath.GetCharCount() ) )
        {
            const char character = filePath[charIndex];

            if ( ( character == '/' || character == '\\' ) && charIndex != filePath.GetCharCount() - 1 )
            {
                return charIndex + 1;
            }
        }

        return 0;
    }

    Count Path::GetExtensionCharacterCount( const primitive::StringView filePath )
    {
        for ( const auto charIndex : foundation::GetReverseCountIterator( filePath.GetCharCount() ) )
        {
            const char character = filePath[charIndex];

            if ( character == '/' || character == '\\' )
            {
                break;
            }
            else if ( character == '.' )
            {
                return filePath.GetCharCount() - charIndex;
            }
        }

        return 0;
    }

    primitive::StringView Path::GetStem( const primitive::StringView filePath )
    {
        const Count directoryCharCount = GetDirectoryCharacterCount( filePath );
        const Count extensionCharCount = GetExtensionCharacterCount( filePath );
        const Count nameCharCount = filePath.GetCharCount() - directoryCharCount - extensionCharCount;

        if ( nameCharCount > 0 )
        {
            return { filePath.GetSubText() + directoryCharCount, nameCharCount };
        }
        else
        {
            return {};
        }
    }

    primitive::StringView Path::GetExtension( const primitive::StringView filePath )
    {
        const Count extensionCharCount = GetExtensionCharacterCount( filePath );

        return { filePath.GetSubText() + filePath.GetCharCount() - extensionCharCount, extensionCharCount };
    }

    primitive::StringView Path::GetDirectory( const primitive::StringView filePath )
    {
        const Count directoryCharCount = GetDirectoryCharacterCount( filePath );

        if ( directoryCharCount > 0 )
        {
            return { filePath.GetSubText(), directoryCharCount };
        }
        else
        {
            return {};
        }
    }

    primitive::StringView Path::GetFileName( const primitive::StringView filePath )
    {
        const Count directoryCharCount = GetDirectoryCharacterCount( filePath );
        const Count nameCharCount = filePath.GetCharCount() - directoryCharCount;

        if ( nameCharCount > 0 )
        {
            return { filePath.GetSubText() + directoryCharCount, nameCharCount };
        }
        else
        {
            return {};
        }
    }

    bool Path::IsPhysical( const primitive::StringView filePath )
    {
        return primitive::ContainsText( filePath, ":/" ) || primitive::StartsWith( filePath, "/" ) ||
               primitive::StartsWith( filePath, "./" ) || primitive::StartsWith( filePath, "../" );
    }

    Path Path::FromPlatformPath( const primitive::StringView path )
    {
        Path result{ path };

        primitive::ReplaceText( result, "\\", "/" );

        result.CanonicalizePath();

        return result;
    }
}