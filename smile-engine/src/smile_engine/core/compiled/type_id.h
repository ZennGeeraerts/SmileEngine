#pragma once

#include "type_name.h"
#include "identifier.h"

namespace smile::compiled
{
    class TypeID final
    {
      public:
        TypeID() = default;

        constexpr TypeID( const std::string_view &text ) : hashCode( identifier::getHashCode( text ) )
        {
        }

        constexpr bool operator==( const TypeID other ) const
        {
            return hashCode == other.hashCode;
        }

        constexpr bool operator!=( const TypeID other ) const
        {
            return hashCode != other.hashCode;
        }

        constexpr bool operator<( const TypeID other ) const
        {
            return hashCode < other.hashCode;
        }

        inline constexpr size_t getHashCode() const
        {
            return hashCode;
        }

      private:
        size_t hashCode;
    };
}

namespace smile
{
    template < typename Type >
    constexpr compiled::TypeID typeIDOf()
    {
        return compiled::TypeID{ typeNameOf< Type >() };
    }

    template < int character_count >
    constexpr compiled::TypeID typeIDOf( const char ( &type )[character_count] )
    {
        return compiled::TypeID{ type };
    }
}

namespace std
{
    template <>
    struct hash< smile::compiled::TypeID >
    {
        size_t operator()( const smile::compiled::TypeID &type_id ) const
        {
            return type_id.getHashCode();
        }
    };
}