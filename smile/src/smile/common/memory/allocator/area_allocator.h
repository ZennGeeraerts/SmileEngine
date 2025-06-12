/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "allocator.h"

namespace smile::memory
{
    class AreaAllocator : public Allocator
    {
      public:
        AreaAllocator() = default;
        ~AreaAllocator() override;

        AreaAllocator( const AreaAllocator & ) = delete;
        AreaAllocator( AreaAllocator && ) = delete;
        AreaAllocator &operator=( const AreaAllocator & ) = delete;
        AreaAllocator &operator=( AreaAllocator && ) = delete;

        bool GetOwnsMemory() const
        {
            return m_OwnsMemory;
        }

        Uint32 GetAllocatedSize() const
        {
            return m_AllocatedSize;
        }

        void *GetAllocatedByteArray() const
        {
            return m_pAllocatedByteArray;
        }

        bool HasAllocatedByteArray() const
        {
            return m_pAllocatedByteArray != nullptr;
        }

        bool Contains( void *pAddress ) const
        {
            return ( UintPointer( pAddress ) >= UintPointer( m_pAllocatedByteArray ) ) &&
                   ( UintPointer( pAddress ) < UintPointer( m_pAllocatedByteArray ) + m_AllocatedSize );
        }

      protected:
        virtual void Initialize( void *pAllocatedByteArray, const Uint32 allocatedSize, bool ownsMemory );

      protected:
        void *m_pAllocatedByteArray{ nullptr };
        Uint32 m_AllocatedSize{ 0 };
        bool m_OwnsMemory{ true };
    };
}