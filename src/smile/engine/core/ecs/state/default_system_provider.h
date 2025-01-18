/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once
#include "system_provider.h"

namespace smile::ecs::state
{
    template < typename ItemType, typename... Args >
    class DefaultSystemProvider final : public SystemProvider
    {
      public:
        DefaultSystemProvider( Args... args ) : m_Args{ args... }
        {
        }
        ~DefaultSystemProvider() = default;

        Ref< BaseSystem > Create() override
        {
            return Ref< BaseSystem >{ std::apply( Construct, m_Args ) };
        }

        static ItemType *Construct( Args &...args )
        {
            return new ItemType{ args... };
        }

      private:
        std::tuple< Args... > m_Args;
    };
}