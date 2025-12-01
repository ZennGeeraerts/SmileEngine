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
 * @created     1 December 2025
 * @brief       Physical file system functions
 */
#include "smpch.h"
#include "physical_system.h"

#include <filesystem>

namespace smile::fs
{
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