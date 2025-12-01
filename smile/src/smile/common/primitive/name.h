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
 * @file        name.h
 * @author      Zenn Geeraerts
 * @created     1 December 2025
 * @brief       Provides a pointer to a string of a named resource
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/identifier.h"

namespace smile::primitive
{
    class String;
    class StringView;
}

namespace smile::stream
{
    class CharStream;
}

namespace smile::primitive
{
    class Name final
    {
      public:
        Name() noexcept = default;

        explicit Name( const char *name ) noexcept;
        explicit Name( const String &name );
        explicit Name( const StringView text );
        explicit Name( const UintPointer nameIndex );

        ~Name() noexcept = default;

        Name &operator=( const String &name );
        Name &operator=( const char *name );

        bool operator==( const Name &other ) const
        {
            return m_pText == other.m_pText;
        }

        bool operator!=( const Name &other ) const
        {
            return m_pText != other.m_pText;
        }

        bool operator<( const Name &other ) const
        {
            return m_pText < other.m_pText;
        }

        const char *GetData() const;

        bool IsEmpty() const
        {
            return m_pText == nullptr;
        }

        const String &GetText() const;

        UintPointer GetIndex() const
        {
            return reinterpret_cast< UintPointer >( m_pText );
        }

        void Clear()
        {
            m_pText = nullptr;
        }

      private:
        const String *m_pText;
    };
}