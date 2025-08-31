/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

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

            if ( decreasedRefCount == 0 )
            {
                delete this;
            }
        }

        Count GetRefCount() const
        {
            return m_RefCount;
        }

      private:
        mutable std::atomic< Count > m_RefCount;
    };
}