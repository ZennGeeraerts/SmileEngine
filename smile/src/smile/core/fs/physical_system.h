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
 * @file        physical_system.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Physical file system functions
 */
#pragma once

#include "path.h"

namespace smile::fs
{
    class PhysicalSystem final
    {
      public:
        PhysicalSystem() = delete;
        ~PhysicalSystem() = delete;

        static void Initialize();

        static Path GetCurrentDirectory();
        static void SetCurrentDirectory( const Path &path );

        static bool DoesFileExist( const Path &path );
        static bool DoesDirectoryExist( const Path &path );
        static bool CreateDirectory( const Path &path );
    };
}