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

namespace smile::fs
{
    Path FileDescriptor::GetPhysicalFilePath() const
    {
        return ( stream::GetTextBuilder() << PhysicalDirectoryPath << PhysicalName ).GetText();
    }
}