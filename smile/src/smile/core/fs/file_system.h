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
#include "smile/common/foundation/flags.h"
#include "smile/common/memory/counted.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/stream/binary_stream.h"
#include "path.h"
#include "file_descriptor_list.h"
#include "recursivity.h"

#include <optional>

namespace smile::fs
{
    enum class MountOption
    {
        ExcludeFiles,
        ExcludeDirectories,
        Writable,
        VirtualCreateIfMissing
    };

    using MountOptionFlags = foundation::Flags< MountOption >;

    class FileSystem final : public memory::Counted, public foundation::Singleton< FileSystem >
    {
      public:
        FileSystem() = default;
        ~FileSystem() = default;

        FileSystem( const FileSystem & ) = delete;
        FileSystem( FileSystem && ) noexcept = delete;

        FileSystem &operator=( const FileSystem & ) = delete;
        FileSystem &operator=( FileSystem && ) noexcept = delete;

        static void CreateInstance() = delete;
        static void Initialize();

        void AddRootDirectory( const Path &rootDirectory );
        void RemoveRootDirectory( const Path &rootDirectory );
        void ClearRootDirectories();

        std::optional< primitive::String > GetFileContent( const Path &filePath ) const;
        BoolResult GetFileBinaryContent( primitive::Vector< Byte > &content, const Path &filePath ) const;

        stream::BinaryStream::Ref GetFile( const Path &filePath ) const;

        BoolResult MountDirectory( const Path &directoryPath,
            const Recursivity recursivity,
            const MountOptionFlags options = {},
            const Path &logicalPath = {} );

        std::optional< Path > FindPhysicalFilePath( const Path &filePath ) const;

      private:
        bool MountDirectoryInternal( const Path &directoryPath,
            const Recursivity recursivity,
            const MountOptionFlags options,
            const Path &logicalPath );

        FileDescriptorList m_FileDescriptorList;
        primitive::Vector< Path > m_RootDirectories;
    };
}