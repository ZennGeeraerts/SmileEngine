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
            return uuid;
        }

      private:
        Uint64 uuid;
    };
}

namespace std
{
    template <>
    struct hash< smile::UUID >
    {
        size_t operator()( const smile::UUID &uuid ) const
        {
            return hash< uint64_t >()( static_cast< uint64_t >( uuid ) );
        }
    };
}