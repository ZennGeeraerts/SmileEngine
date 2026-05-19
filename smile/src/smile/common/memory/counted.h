/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "memory.h"

#include <atomic>

namespace smile::memory
{
    class Counted
    {
      public:
        Counted() : m_RefCount{ 0 }
        {
        }

        Counted( const Counted & ) : m_RefCount{ 0 }
        {
        }

        virtual ~Counted();

        Counted &operator=( const Counted & )
        {
            return *this;
        }

        void *operator new( const size_t size );

        void *operator new( const size_t, void *byteArray, memory::InPlace * ) noexcept
        {
            return byteArray;
        }

        void operator delete( void *byteArray );

        // This needs to be const to work in Reset function of Object
        inline void IncreaseRefCount() const
        {
            SM_ASSERT_MSG( m_RefCount != s_InvalidCount, "Object is destructed" );
            ++m_RefCount;
        }

        // This needs to be const to work in Reset function of Object
        inline void DecreaseRefCount() const
        {
            const Count decreasedRefCount = --m_RefCount;

            if ( decreasedRefCount == 0 && IsAllocated() )
            {
                this->~Counted();

                if ( !m_LinkCount )
                {
                    memory::DeallocateByteArray( const_cast< Counted * >( this ) );
                }
            }
        }

        inline void IncreaseLinkCount() const
        {
            SM_ASSERT_MSG( m_RefCount != s_InvalidCount, "Object is destructed" );
            ++m_LinkCount;
        }

        inline void DecreaseLinkCount() const
        {
            const Count decreasedLinkCount = --m_LinkCount;

            if ( decreasedLinkCount == 0 && m_RefCount == s_InvalidCount && IsAllocated() )
            {
                memory::DeallocateByteArray( const_cast< Counted * >( this ) );
            }
        }

        Count GetRefCount() const noexcept
        {
            return m_RefCount;
        }

        Count GetLinkCount() const noexcept
        {
            return m_LinkCount;
        }

        bool IsAllocated() const
        {
            return memory::IsAllocatedObject( this );
        }

        bool IsDestructed() const noexcept
        {
            return m_RefCount == s_InvalidCount;
        }

      private:
        mutable std::atomic< Count > m_RefCount;
        mutable std::atomic< Count > m_LinkCount;
    };
}