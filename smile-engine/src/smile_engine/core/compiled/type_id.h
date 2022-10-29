#pragma once

#include "type_name.h"
#include "identifier.h"

namespace Smile::Compiled
{
    class TypeID final
    {
      public:
        TypeID() = default;

        constexpr TypeID( const std::string_view &text ) : m_HashCode( Identifier::GetHashCode( text ) )
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
}

namespace Smile
{
    template < typename Type >
    constexpr Compiled::TypeID TypeIDOf()
    {
        return Compiled::TypeID{ TypeNameOf< Type >() };
    }

    template < int characterCount >
    constexpr Compiled::TypeID TypeIDOf( const char ( &type )[characterCount] )
    {
        return Compiled::TypeID{ type };
    }
}

namespace std
{
    template <>
    struct hash< Smile::Compiled::TypeID >
    {
        size_t operator()( const Smile::Compiled::TypeID &typeID ) const
        {
            return typeID.GetHashCode();
        }
    };
}