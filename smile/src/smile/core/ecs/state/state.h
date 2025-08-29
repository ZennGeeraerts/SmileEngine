/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/counted.h"
#include "smile/common/primitive/text/string.h"
#include "smile/common/primitive/collection/vector.h"

namespace smile::ecs::state
{
    class State final : public memory::Counted
    {
      public:
        using Iterator = primitive::Vector< primitive::String >::Iterator;
        using ConstIterator = primitive::Vector< primitive::String >::ConstIterator;

      public:
        State() = default;
        ~State() = default;

        void AddSystem( primitive::String systemName );
        bool HasSystem( const primitive::StringView systemName );

        const primitive::Vector< primitive::String > &GetSystemNames() const
        {
            return m_SystemNames;
        }

      private:
        primitive::Vector< primitive::String > m_SystemNames;
    };
}