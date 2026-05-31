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

        const FileDescriptor &operator[]( const Index index ) const noexcept
        {
            return m_Descriptors[index];
        }

        bool IsEmpty() const noexcept
        {
            return m_Descriptors.IsEmpty();
        }

        Count GetItemCount() const noexcept
        {
            return m_Descriptors.GetItemCount();
        }

        void SetCaseSensitive( bool isCaseSensitive ) noexcept
        {
            m_IsCaseSensitive = isCaseSensitive;
        }

        bool IsCaseSensitive() const noexcept
        {
            return m_IsCaseSensitive;
        }

        auto begin() const noexcept
        {
            return m_Descriptors.begin();
        }

        auto begin() noexcept
        {
            return m_Descriptors.begin();
        }

        auto end() const noexcept
        {
            return m_Descriptors.end();
        }

        auto end() noexcept
        {
            return m_Descriptors.end();
        }

        std::optional< Path > FindPhysicalFilePath( const Path &filePath ) const;
        std::optional< FileDescriptor > FindDescriptor( const Path &filePath ) const;

        void Merge( primitive::Vector< FileDescriptor > &descriptorTable );

      private:
        primitive::Vector< FileDescriptor > m_Descriptors;
        bool m_IsCaseSensitive{ true };
    };
}