#pragma once

#include <xhash>

namespace smile
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
    struct hash< smile::UUID >
    {
        size_t operator()( const smile::UUID &uuid ) const
        {
            return hash< smile::Uint64 >()( static_cast< smile::Uint64 >( uuid ) );
        }
    };
}