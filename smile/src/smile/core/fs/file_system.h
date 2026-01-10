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
 * @file        file_system.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Managing files
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/singleton.h"
#include "smile/common/memory/counted.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/stream/binary_stream.h"
#include "path.h"
#include "file_descriptor_list.h"

#include <optional>

namespace smile::fs
{
    class FileSystem final : public memory::Counted, public foundation::Singleton< FileSystem >
    {
      public:
        FileSystem() = default;
        ~FileSystem() = default;

        static void CreateInstance() = delete;
        static void Initialize();

        void AddRootDirectory( const Path &rootDirectory );
        void RemoveRootDirectory( const Path &rootDirectory );
        void ClearRootDirectories();

        std::optional< primitive::String > GetFileContent( const Path &filePath ) const;
        bool GetFileBinaryContent( primitive::Vector< Byte > &content, const Path &filePath ) const;

        stream::BinaryStream::Ref GetFile( const Path &filePath ) const;

      private:
        FileDescriptorList m_FileDescriptorList;
        primitive::Vector< Path > m_RootDirectories;
    };
}