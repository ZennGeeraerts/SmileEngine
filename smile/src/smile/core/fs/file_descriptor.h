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
 * @file        file_descriptor.h
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Describes info about a file
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/name.h"
#include "type.h"
#include "path.h"

namespace smile::fs
{
    class FileDescriptor final
    {
      public:
        FileDescriptor() = default;
        FileDescriptor( const FileDescriptor & ) = default;
        FileDescriptor( FileDescriptor && ) = default;
        ~FileDescriptor() = default;

        FileDescriptor &operator=( const FileDescriptor & ) = default;
        FileDescriptor &operator=( FileDescriptor && ) = default;

        bool operator==( const FileDescriptor &other ) const
        {
            return PhysicalName == other.PhysicalName && LogicalName == other.LogicalName &&
                   PhysicalDirectoryPath == other.PhysicalDirectoryPath &&
                   LogicalDirectoryPath == other.LogicalDirectoryPath;
        }

        bool operator!=( const FileDescriptor &other ) const
        {
            return !( *this == other );
        }

        Path GetPhysicalFilePath() const;

        primitive::String PhysicalName;
        primitive::String LogicalName;
        primitive::Name PhysicalDirectoryPath;
        primitive::Name LogicalDirectoryPath;

        Count Size{ s_InvalidCount };
        Type FileType;
        bool IsReadOnly{ true };
        bool IsVirtual{ false };
    };
}