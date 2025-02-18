/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include <atomic>

namespace smile::memory
{
    class Object
    {
      public:
        Object() : m_RefCount{ 0 }
        {
        }

        Object( const Object & ) : m_RefCount{ 0 }
        {
        }

        virtual ~Object();

        inline void IncreaseRefCount()
        {
            SM_ASSERT( m_RefCount != DestructedObjectRefCount(), "Object is destructed" );
            ++m_RefCount;
        }

        inline void DecreaseRefCount()
        {
            Uint32 decreasedRefCount = --m_RefCount;

            if ( decreasedRefCount == 0 )
            {
                this->~Object();
            }
        }

        Uint32 GetRefCount() const
        {
            return m_RefCount;
        }

        static constexpr Uint32 DestructedObjectRefCount();

      private:
        std::atomic< Uint32 > m_RefCount;
    };
}