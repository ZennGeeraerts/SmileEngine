/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "type_name.h"
#include "identifier.h"

namespace smile::stl
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

        inline constexpr size_t GetHashCode() const
        {
            return m_HashCode;
        }

      private:
        size_t m_HashCode;
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
    struct hash< smile::stl::TypeID >
    {
        size_t operator()( const smile::stl::TypeID &typeID ) const
        {
            return typeID.GetHashCode();
        }
    };
}