/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/memory/memory.h"
#include "area_allocator.h"

namespace smile::memory
{
    class StackAllocator final : public AreaAllocator
    {
      public:
        StackAllocator() = default;
        ~StackAllocator() override;

        StackAllocator( const StackAllocator & ) = delete;
        StackAllocator( StackAllocator && ) = delete;
        StackAllocator &operator=( const StackAllocator & ) = delete;
        StackAllocator &operator=( StackAllocator && ) = delete;

        Uint32 GetAllocationCount() const
        {
            return m_AllocationCount;
        }

        Uint32 GetAllocatedSize() const
        {
            return m_InternalAllocatedSize;
        }

        Uint32 GetAvailableSize() const
        {
            return m_AvailableSize;
        }

        void *GetAllocatedByteArray()
        {
            return m_pAllocatedByteArray;
        }

        const void *GetAllocatedByteArray() const
        {
            return m_pAllocatedByteArray;
        }

        bool CanAllocateByteArray( const Uint32 size ) const override;

        bool IsEmpty() const
        {
            return m_AllocatedSize == m_AvailableSize;
        }

        void Initialize( void *pAllocatedByteArray, const Uint32 allocatedSize, bool ownsMemory ) override;
        void Reset();

      private:
        void *InternalCreateByteArray( const Uint32 size, const void *pExtraData ) override;
        void InternalDestroyByteArray( void *pByteArray, const Uint32 size ) override;

      private:
        Uint32 m_AvailableSize{ 0 };
        Uint32 m_InternalAllocatedSize{ 0 };
        Byte *m_pCurrentByteArray{ nullptr };
        Uint32 m_AllocationCount{ 0 };
    };
}