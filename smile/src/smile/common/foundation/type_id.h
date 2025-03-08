/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "type_name.h"
#include "identifier.h"

namespace smile::foundation
{
    class TypeID final
    {
      public:
        TypeID() = default;

        constexpr TypeID( const std::string_view &text ) : m_HashCode( identifier::GetHashCode( text ) )
        {
        }

        constexpr bool operator==( const TypeID other ) const
        {
            return m_HashCode == other.m_HashCode;
        }

        constexpr bool operator!=( const TypeID other ) const
        {
            return m_HashCode != other.m_HashCode;
        }

        constexpr bool operator<( const TypeID other ) const
        {
            return m_HashCode < other.m_HashCode;
        }

        inline constexpr HashCode GetHashCode() const
        {
            return m_HashCode;
        }

      private:
        HashCode m_HashCode;
    };

    template < typename Type >
    constexpr TypeID TypeIDOf()
    {
        return TypeID{ TypeNameOf< Type >() };
    }

    template < int characterCount >
    constexpr TypeID TypeIDOf( const char ( &type )[characterCount] )
    {
        return TypeID{ type };
    }
}

namespace std
{
    template <>
    struct hash< smile::foundation::TypeID >
    {
        size_t operator()( const smile::foundation::TypeID &typeID ) const
        {
            return typeID.GetHashCode();
        }
    };
}