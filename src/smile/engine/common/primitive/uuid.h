/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/compiled/compiled.h"
#include <xhash>

namespace smile::primitive
{
    class UUID final
    {
      public:
        UUID();
        UUID( Uint64 id );
        UUID( const UUID & ) = default;

        operator Uint64() const
        {
            return m_UUID;
        }

      private:
        Uint64 m_UUID;
    };
}

namespace std
{
    template <>
    struct hash< smile::primitive::UUID >
    {
        size_t operator()( const smile::primitive::UUID &uuid ) const
        {
            return hash< smile::Uint64 >()( static_cast< smile::Uint64 >( uuid ) );
        }
    };
}