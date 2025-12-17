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
#include "smile/common/primitive/collection/array_utils.h"

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

    bool FileSystem::GetFileBinaryContent( primitive::Vector< Byte > &content, const Path &filePath ) const
    {
        auto pFile = GetFile( filePath );

        if ( !pFile )
        {
            SM_LOG_WARNING( "Unable to find file: {}", filePath );
            return false;
        }

        pFile->OpenInput();
        content.SetItemCount( pFile->GetSize() );
        pFile->ReadByteArray( content.GetData(), pFile->GetSize() );
        pFile->Close();

        return true;
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
}