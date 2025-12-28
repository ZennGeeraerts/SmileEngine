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
 * @file        name_manager.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stores a global table of names
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/hash_table.h"
#include "smile/common/primitive/text/string.h"

#include <mutex>

namespace smile::primitive
{
    class NameManager final
    {
      public:
        NameManager() noexcept = default;
        NameManager( const NameManager & ) noexcept = delete;
        NameManager( NameManager && ) noexcept = delete;
        ~NameManager() noexcept = default;

        NameManager &operator=( const NameManager & ) noexcept = delete;
        NameManager &operator=( NameManager && ) noexcept = delete;

        const String *GetName( const char *name );

        static NameManager &GetInstance();

      private:
        struct Node final
        {
            using KeyType = String;
            using ItemType = const String *;

            void SetNextNode( Node *pNode )
            {
                pNextNode = pNode;
            }

            Node *GetNextNode() const
            {
                return pNextNode;
            }

            const KeyType &GetKey() const
            {
                return Text;
            }

            const String *GetItem() const
            {
                return &Text;
            }

            foundation::HashCode GetHashCode() const
            {
                return Hash;
            }

            Node *pNextNode;
            foundation::HashCode Hash;
            String Text;
        };

        HashTable< Node > m_NameTable;
        std::mutex m_Mutex;
    };
}