#pragma once

#include <xhash>

namespace smile
{
    class UUID final
    {
      public:
        UUID();
        UUID( uint64_t id );
        UUID( const UUID & ) = default;

        operator uint64_t() const
        {
            return m_UUID;
        }

      private:
        uint64_t m_UUID;
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