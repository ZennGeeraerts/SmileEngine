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
 * @file        name.cpp
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Provides a pointer to a string of a named resource
 */
#include "smpch.h"
#include "smile/common/primitive/name.h"

#include "smile/common/primitive/name/name_manager.h"

namespace smile::primitive
{
    Name::Name( const char *name ) noexcept : m_pText{ NameManager::GetInstance().GetName( name ) }
    {
    }

    Name::Name( const String &name ) : m_pText{ NameManager::GetInstance().GetName( name.GetData() ) }
    {
    }

    Name::Name( const UintPointer nameIndex ) : m_pText{ reinterpret_cast< const String * >( nameIndex ) }
    {
    }

    Name::Name( const StringView text ) : Name{ String{ text } }
    {
    }

    Name &Name::operator=( const String &name )
    {
        m_pText = NameManager::GetInstance().GetName( name.GetData() );

        return *this;
    }

    Name &Name::operator=( const char *name )
    {
        m_pText = NameManager::GetInstance().GetName( name );

        return *this;
    }

    const String &Name::GetText() const
    {
        return m_pText ? *m_pText : "";
    }

    const char *Name::GetData() const
    {
        return m_pText ? m_pText->GetData() : "";
    }
}