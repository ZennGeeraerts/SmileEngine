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
 * @file        file.h
 * @author      Zenn Geeraerts
 * @created     28 November 2025
 * @brief       File IO operations
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/stream/binary_stream.h"
#include "path.h"

#include <fstream>

namespace smile::fs
{
    class File : public stream::BinaryStream
    {
      public:
        File( const Path &path ) noexcept;
        ~File() override = default;

        bool SetIndex( const Index index ) override;
        bool OpenInput() override;
        bool OpenOutput( stream::OpeningModeFlags openingModeFlags ) override;
        bool OpenInputOutput( stream::OpeningModeFlags openingModeFlags ) override;

        Count ReadByteArray( void *pByteArray, Count size ) override;
        Count WriteByteArray( const void *pByteArray, const Count size ) override;

        bool Close() override;

        const Path &GetPath() const
        {
            return m_Path;
        }

      private:
        bool Open( bool isInput, bool isOutput, stream::OpeningModeFlags flags );

      private:
        std::fstream m_FileStream;
        Path m_Path;
    };
}