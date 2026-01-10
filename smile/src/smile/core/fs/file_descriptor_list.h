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
 * @file        file_descriptor_list.h
 * @author      Zenn Geeraerts
 * @created     30 December 2025
 * @brief       List of file descriptors
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "file_descriptor.h"

namespace smile::fs
{
    class FileDescriptorList final
    {
      public:
        FileDescriptorList() = default;
        FileDescriptorList( const FileDescriptorList & ) = delete;
        FileDescriptorList( FileDescriptorList && ) = delete;
        ~FileDescriptorList() = default;

        FileDescriptorList &operator=( const FileDescriptorList & ) = delete;
        FileDescriptorList &operator=( FileDescriptorList && ) = delete;

        const FileDescriptor &operator[]( const Index index ) const
        {
            return m_Descriptors[index];
        }

        bool IsEmpty() const
        {
            return m_Descriptors.IsEmpty();
        }

        Count GetItemCount() const
        {
            return m_Descriptors.GetItemCount();
        }

        bool SetCaseSensitive( bool isCaseSensitive )
        {
            m_IsCaseSensitive = isCaseSensitive;
        }

        bool IsCaseSensitive() const
        {
            return m_IsCaseSensitive;
        }

        auto begin() const
        {
            return m_Descriptors.begin();
        }

        auto begin()
        {
            return m_Descriptors.begin();
        }

        auto end() const
        {
            return m_Descriptors.end();
        }

        auto end()
        {
            return m_Descriptors.end();
        }

      private:
        primitive::Vector< FileDescriptor > m_Descriptors;
        bool m_IsCaseSensitive{ true };
    };
}