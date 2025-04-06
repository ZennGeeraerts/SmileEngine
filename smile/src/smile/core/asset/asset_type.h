/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/type_id.h"
#include "smile/common/foundation/type_registry.h"

namespace smile::asset
{
    class AssetType final
    {
      public:
        constexpr AssetType() : m_TypeID{ "" }
        {
        }

        explicit AssetType( const std::string &name )
            : m_TypeID{ foundation::TypeRegistry::GetInstance().RegisterTypeIfNeeded( name ) }
        {
        }

        bool IsValid() const
        {
            return foundation::TypeRegistry::GetInstance().Contains( m_TypeID );
        }

        bool operator==( const AssetType other ) const
        {
            return m_TypeID == other.m_TypeID;
        }

        bool operator!=( const AssetType other ) const
        {
            return m_TypeID != other.m_TypeID;
        }

        bool operator<( const AssetType other ) const
        {
            return m_TypeID < other.m_TypeID;
        }

        constexpr foundation::HashCode GetHashCode() const
        {
            return m_TypeID.GetHashCode();
        }

        std::string_view GetName() const
        {
            return foundation::TypeRegistry::GetInstance().GetName( m_TypeID );
        }

        static constexpr AssetType NullType();

      private:
        foundation::TypeID m_TypeID;
    };

    constexpr AssetType AssetType::NullType()
    {
        return AssetType{};
    }
}