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
 * @file        physical_system.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Physical file system functions
 */
#include "smpch.h"
#include "physical_system.h"

#include "smile/common/primitive/text/utils.h"
#include "file_system.h"

#include <filesystem>

namespace smile::fs
{
    namespace detail
    {
        void GetFileTable( primitive::Vector< Path > &fileTable,
            primitive::Vector< Path > *directoryTable,
            const char *path,
            const Recursivity recursivity )
        {
            std::error_code errorCode;
            for ( const auto &entry : std::filesystem::directory_iterator( path, errorCode ) )
            {
                if ( entry.is_directory( errorCode ) && !errorCode )
                {
                    if ( directoryTable )
                    {
#ifdef SM_COMPILER_MSVC
                        primitive::String platformPath{ entry.path().string().c_str() };
                        primitive::ReplaceText( platformPath, "\\", "/" );
                        directoryTable->EmplaceBack( std::move( platformPath ) );
#else
                        directoryTable->EmplaceBack( entry.path().string().c_str() );
#endif
                    }

                    if ( recursivity == Recursivity::Recursive )
                    {
                        GetFileTable( fileTable, directoryTable, entry.path().string().c_str(), recursivity );
                    }
                }
                else if ( entry.is_regular_file( errorCode ) && !errorCode )
                {
#ifdef SM_COMPILER_MSVC
                    primitive::String platformPath{ entry.path().string().c_str() };
                    primitive::ReplaceText( platformPath, "\\", "/" );
                    fileTable.EmplaceBack( std::move( platformPath ) );
#else
                    fileTable.EmplaceBack( entry.path().string().c_str() );
#endif
                }
            }
        }
    }

    void PhysicalSystem::Initialize()
    {
        FileSystem::GetInstance().AddRootDirectory( GetCurrentDirectory() );
    }

    void PhysicalSystem::GetFileTable( primitive::Vector< Path > &fileTable,
        primitive::Vector< Path > *directoryTable,
        const Path &path,
        const Recursivity recursivity )
    {
        detail::GetFileTable( fileTable, directoryTable, path.GetData(), recursivity );
    }

    Path PhysicalSystem::GetCurrentDirectory()
    {
        return Path::FromPlatformPath( std::filesystem::current_path().string().c_str() ) + "/";
    }

    void PhysicalSystem::SetCurrentDirectory( const Path &path )
    {
        std::filesystem::current_path( path.GetData() );
    }

    Path PhysicalSystem::GetAbsolutePath( const Path &path )
    {
        return Path::FromPlatformPath( std::filesystem::absolute( path.GetData() ).string().c_str() );
    }

    bool PhysicalSystem::DoesFileExist( const Path &path )
    {
        std::error_code errorCode;
        const auto fileStatus = std::filesystem::status( path.GetData(), errorCode );

        return !errorCode && std::filesystem::is_regular_file( fileStatus );
    }

    bool PhysicalSystem::DoesDirectoryExist( const Path &path )
    {
        std::error_code errorCode;
        const auto fileStatus = std::filesystem::status( path.GetData(), errorCode );

        return !errorCode && std::filesystem::is_directory( fileStatus );
    }

    bool PhysicalSystem::CreateDirectory( const Path &path )
    {
        std::error_code errorCode;
        std::filesystem::create_directories( path.GetData(), errorCode );
        return !errorCode;
    }
}