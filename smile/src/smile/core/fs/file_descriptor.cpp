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
 * @file        file_descriptor.cpp
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Describes info about a file
 */
#include "smpch.h"
#include "file_descriptor.h"

#include "smile/common/stream/text_stream.h"
#include "smile/common/primitive/text/utils.h"
#include "smile/common/foundation/identifier.h"

namespace smile::fs
{
    namespace detail
    {
        void ExtractFileName( primitive::String &logicalName,
            Compression &compression,
            const primitive::StringView fileName )
        {
            const auto extension = Path::GetExtension( fileName );

            switch ( foundation::identifier::GetHashCodeWithSize( extension.GetSubText(), extension.GetCharCount() ) )
            {
                case ".lz4"_id:
                    compression = Compression::Lz4;
                    logicalName =
                        primitive::GetSubText( fileName, 0, fileName.GetCharCount() - extension.GetCharCount() );
                    break;

                default:
                    compression = Compression::None;
                    logicalName = fileName;
                    break;
            }
        }
    }

    Path FileDescriptor::GetPhysicalFilePath() const
    {
        return ( stream::GetTextBuilder() << PhysicalDirectoryPath << PhysicalName ).GetText();
    }

    void FileDescriptor::FillPhysical( const Path &filePath,
        const Type type,
        const bool isCaseSensitive,
        const std::optional< Count > size,
        const primitive::String &logicalDirectoryPath,
        const primitive::String &physicalDirectoryPath )
    {
        PhysicalName = filePath.GetFileName();

        LogicalDirectoryPath = [&logicalDirectoryPath, isCaseSensitive]()
        {
            if ( isCaseSensitive )
            {
                return logicalDirectoryPath;
            }
            else
            {
                primitive::String lowerCaseLogicalDirectoryPath{ logicalDirectoryPath };
                primitive::ToLowerCase( lowerCaseLogicalDirectoryPath );
                return lowerCaseLogicalDirectoryPath;
            }
        }();

        detail::ExtractFileName( LogicalName, FileCompression, filePath.GetFileName() );

        if ( !isCaseSensitive )
        {
            primitive::ToLowerCase( LogicalName );
        }

        if ( !physicalDirectoryPath.IsEmpty() )
        {
            PhysicalDirectoryPath = physicalDirectoryPath;
        }
        else
        {
            PhysicalDirectoryPath = primitive::Name{ filePath.GetDirectory() };
        }

        FileType = type;
        Size = size.value_or( s_InvalidCount );
    }
}