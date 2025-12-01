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
 * @file        name_manager.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Stores a global table of names
 */
#include "smpch.h"
#include "name_manager.h"

namespace smile::primitive
{
    const String *NameManager::GetName( const char *name )
    {
        if ( *name == 0 )
        {
            return nullptr;
        }

        {
            std::lock_guard _{ m_Mutex };

            const auto instance = m_NameTable.FindItemAtKey( name );

            if ( instance != m_NameTable.end() )
            {
                return ( *instance ).Value;
            }
            else
            {
                Node *pNode = new Node{};
                pNode->Text = name;
                pNode->Hash = foundation::identifier::GetHashCode( name );
                m_NameTable.Insert( pNode );

                return &pNode->Text;
            }
        }
    }

    NameManager &NameManager::GetInstance()
    {
        static Byte nameManagerMemory[sizeof( NameManager )];
        static NameManager *pNameManager = nullptr;

        if ( !pNameManager )
        {
            pNameManager = ::new ( nameManagerMemory, memory::g_pInPlace ) NameManager{};
        }

        return *pNameManager;
    }
}