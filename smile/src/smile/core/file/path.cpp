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

namespace smile::file
{
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
}