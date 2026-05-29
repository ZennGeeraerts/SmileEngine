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
 * @file        file_system.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Managing files
 */
#include "smpch.h"
#include "file_system.h"

#include "file.h"
#include "physical_system.h"
#include "smile/common/stream/text_stream.h"
#include "smile/common/primitive/collection/array_utils.h"
#include "smile/common/primitive/text/utils.h"

namespace smile::fs
{
    void FileSystem::Initialize()
    {
        foundation::Singleton< FileSystem >::CreateInstance();

        PhysicalSystem::Initialize();
    }

    void FileSystem::AddRootDirectory( const Path &rootDirectory )
    {
        SM_ASSERT( !primitive::array::HasItem( m_RootDirectories, rootDirectory ) );

        m_RootDirectories.PushBack( rootDirectory );
    }

    void FileSystem::RemoveRootDirectory( const Path &rootDirectory )
    {
        primitive::array::EraseIfExists( m_RootDirectories, rootDirectory );
    }

    void FileSystem::ClearRootDirectories()
    {
        m_RootDirectories.Clear();
    }

    std::optional< primitive::String > FileSystem::GetFileContent( const Path &filePath ) const
    {
        primitive::String content;
        auto pFile = GetFile( filePath );

        if ( !pFile )
        {
            SM_LOG_WARNING( "Unable to find file: {}", filePath );
            return std::nullopt;
        }

        pFile->OpenInput();

        if ( !pFile->IsOpen() )
        {
            SM_LOG_WARNING( "Unable to open file: {}", filePath );
            return std::nullopt;
        }

        stream::GetTextContent( content, *pFile );

        pFile->Close();

        if ( content.IsEmpty() )
        {
            SM_LOG_WARNING( "File: '{}' is empty", filePath );
        }

        return content;
    }

    BoolResult FileSystem::GetFileBinaryContent( primitive::Vector< Byte > &content, const Path &filePath ) const
    {
        auto pFile = GetFile( filePath );

        if ( !pFile )
        {
            SM_LOG_ERROR( "FileSystem::GetFileBinaryContent >> Unable to find file: {}", filePath );
            return BoolResult::Fail( "Unable to find file" );
        }

        pFile->OpenInput();
        content.SetItemCount( pFile->GetSize() );
        pFile->ReadByteArray( content.GetData(), pFile->GetSize() );
        pFile->Close();

        return BoolResult::Succeed();
    }

    stream::BinaryStream::Ref FileSystem::GetFile( const Path &filePath ) const
    {
        if ( PhysicalSystem::DoesFileExist( filePath ) )
        {
            auto pFile = memory::CreateRef< File >( filePath );
            return pFile;
        }

        return nullptr;
    }

    BoolResult FileSystem::MountDirectory( const Path &directoryPath,
        const Recursivity recursivity,
        const MountOptionFlags options,
        const Path &logicalPath )
    {
        if ( directoryPath.IsPhysical() )
        {
            Path smileDirectoryPath = directoryPath;
            Path smileLogicalPath = logicalPath;

            if ( smileDirectoryPath.GetLastChar() != '/' )
            {
                smileDirectoryPath += '/';
            }

            if ( !smileLogicalPath.IsEmpty() && smileLogicalPath.GetLastChar() != '/' )
            {
                smileLogicalPath += '/';
            }

            if ( MountDirectoryInternal( smileDirectoryPath, recursivity, options, smileLogicalPath ) )
            {
                return BoolResult::Succeed();
            }
        }
        else
        {
            for ( const auto &rootDirectory : m_RootDirectories )
            {
                Path smileDirectoryPath = rootDirectory + directoryPath;

                if ( smileDirectoryPath.GetLastChar() != '/' )
                {
                    smileDirectoryPath += '/';
                }

                if ( MountDirectoryInternal( smileDirectoryPath, recursivity, options, logicalPath ) )
                {
                    return BoolResult::Succeed();
                }
            }
        }

        SM_LOG_ERROR( "FileSystem::MountDirectory >> Unable to find directory: {}", directoryPath );

        return BoolResult::Fail( "Directory not found" );
    }

    std::optional< Path > FileSystem::FindPhysicalFilePath( const Path &filePath ) const
    {
        return m_FileDescriptorList.FindPhysicalFilePath( filePath );
    }

    bool FileSystem::MountDirectoryInternal( const Path &directoryPath,
        const Recursivity recursivity,
        const MountOptionFlags options,
        const Path &logicalPath )
    {
        if ( !PhysicalSystem::DoesDirectoryExist( directoryPath ) )
        {
            return false;
        }

        primitive::Vector< Path > fileTable;
        primitive::Vector< Path > directoryTable;
        PhysicalSystem::GetFileTable( fileTable, &directoryTable, directoryPath, recursivity );

        primitive::Vector< FileDescriptor > descriptorTable;
        descriptorTable.Reserve( fileTable.GetItemCount() + directoryTable.GetItemCount() );

        const bool isReadOnly = options.Has( MountOption::Writable );

        auto fillDescriptors = [this, &logicalPath, &directoryPath, &descriptorTable, isReadOnly](
                                   const primitive::Vector< Path > &paths, fs::Type type )
        {
            for ( const auto &filePath : paths )
            {
                stream::TextStream fullDirectoryPath;
                fullDirectoryPath << logicalPath
                                  << primitive::GetEndingTextAtIndex(
                                         filePath.GetDirectory(), directoryPath.GetCharCount() );

                FileDescriptor descriptor;

                descriptor.FillPhysical(
                    filePath, type, m_FileDescriptorList.IsCaseSensitive(), std::nullopt, fullDirectoryPath.GetText() );
                descriptor.IsReadOnly = isReadOnly;

                if ( descriptor.PhysicalName[0] == '.' )
                {
                    SM_LOG_INFO( "FileSystem::MountDirectoryInternal >> Skipping file: {} because it starts with a dot",
                        descriptor.PhysicalName );

                    continue;
                }

                SM_LOG_TRACE( "FileSystem::MountDirectoryInternal >> Mounting: {}{} as {}{}",
                    descriptor.PhysicalDirectoryPath,
                    descriptor.PhysicalName,
                    descriptor.LogicalDirectoryPath,
                    descriptor.LogicalName );

                descriptorTable.PushBack( std::move( descriptor ) );
            }
        };

        if ( !options.Has( MountOption::ExcludeFiles ) )
        {
            fillDescriptors( fileTable, Type::File );
        }

        if ( !options.Has( MountOption::ExcludeDirectories ) )
        {
            if ( !logicalPath.IsEmpty() )
            {
                FileDescriptor descriptor;

                descriptor.FillPhysical( logicalPath,
                    Type::Directory,
                    m_FileDescriptorList.IsCaseSensitive(),
                    std::nullopt,
                    logicalPath.GetDirectory(),
                    directoryPath.GetDirectory() );

                descriptor.IsReadOnly = isReadOnly;

                SM_LOG_TRACE( "FileSystem::MountDirectoryInternal >> Mounting: {}{} as {}{}",
                    descriptor.PhysicalDirectoryPath,
                    descriptor.PhysicalName,
                    descriptor.LogicalDirectoryPath,
                    descriptor.LogicalName );

                descriptorTable.PushBack( std::move( descriptor ) );
            }

            fillDescriptors( directoryTable, Type::Directory );
        }

        if ( !logicalPath.IsEmpty() )
        {
            auto path = directoryPath;
            path.SetCharCount( path.GetCharCount() - 1 );

            FileDescriptor descriptor;

            descriptor.FillPhysical( path,
                Type::Directory,
                m_FileDescriptorList.IsCaseSensitive(),
                std::nullopt,
                logicalPath.GetDirectory() );

            descriptor.IsReadOnly = isReadOnly;

            descriptorTable.PushBack( std::move( descriptor ) );
        }

        m_FileDescriptorList.Merge( descriptorTable );

        return true;
    }
}