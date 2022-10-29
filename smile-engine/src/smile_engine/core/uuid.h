#pragma once

#include <xhash>

namespace Smile
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
    struct hash< Smile::UUID >
    {
        size_t operator()( const Smile::UUID &uuid ) const
        {
            return hash< Smile::Uint64 >()( static_cast< Smile::Uint64 >( uuid ) );
        }
    };
}