/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::memory
{
    class Allocator
    {
      public:
        Allocator();
        virtual ~Allocator();

        Allocator( const Allocator & ) = delete;
        Allocator( Allocator && ) = delete;
        Allocator &operator=( const Allocator & ) = delete;
        Allocator &operator=( Allocator && ) = delete;

        Uint32 GetAllocatorIndex() const
        {
            return m_AllocatorIndex;
        }

        void SetUseHeader( const bool useHeader )
        {
            m_UseHeader = useHeader;
        }

        bool GetUseHeader() const
        {
            return m_UseHeader;
        }

        void SetBreaksOnOutOfMemory( const bool breaksOnOutOfMemory )
        {
            m_BreaksOnOutOfMemory = breaksOnOutOfMemory;
        }

        bool GetBreaksOnOutOfMemory() const
        {
            return m_BreaksOnOutOfMemory;
        }

        Uint64 GetAllocatedSize() const
        {
#if SM_C_DEBUG
            return m_AllocatedSize.load();
#else
            return 0;
#endif
        }

        virtual bool CanAllocateByteArray( const Uint32 size ) const = 0;

        void *CreateByteArray( const Uint32 size );
        void DestroyByteArray( void *pByteArray );

        void *CreateAlignedByteArray( const Uint32 size, const Uint32 alignedSize );
        void DestroyAlignedByteArray( void *pByteArray );

        static void AddAllocator( Allocator &allocator );
        static void RemoveAllocator( Allocator &allocator );

        static Allocator &GetAllocatorByIndex( const Uint32 index )
        {
            SM_ASSERT( HasAllocatorAtIndex( index ) );

            return *s_pAllocators[index];
        }

        static Uint32 GetAllocatorCount();

        static bool HasAllocatorAtIndex( const Uint32 index )
        {
            UintPointer indexValue = UintPointer( s_pAllocators[index] );

            return indexValue > s_MaxAllocatorCount &&
                   indexValue != UintPointer( std::numeric_limits< Uint32 >::max() );
        }

      protected:
        void *CreateByteArray( const Uint32 size, const void *pExtraData );
        void *CreateAlignedByteArray( const Uint32 size, const Uint32 alignedSize, const void *pExtraData );

        virtual void *InternalCreateByteArray( const Uint32 size, const void *pExtraData ) = 0;
        virtual void InternalDestroyByteArray( void *pByteArray, const Uint32 size ) = 0;

      protected:
        Uint32 m_AllocatorIndex;
        bool m_UseHeader : 1, m_BreaksOnOutOfMemory : 1;
#if SM_C_DEBUG
        std::atomic< Uint64 > m_AllocatedSize;
#endif

        constexpr static Uint32 s_MaxAllocatorCount = 1024;

      private:
        static Allocator *s_pAllocators[s_MaxAllocatorCount];
    };

    inline void *Allocator::CreateByteArray( const Uint32 size )
    {
        return CreateByteArray( size, nullptr );
    }

    inline void *Allocator::CreateAlignedByteArray( const Uint32 size, const Uint32 alignedSize )
    {
        return CreateAlignedByteArray( size, alignedSize, nullptr );
    }
}

inline void *operator new( std::size_t size, smile::memory::Allocator &allocator ) noexcept
{
    return allocator.CreateByteArray( static_cast< smile::Uint32 >( size ) );
}

inline void *operator new[]( std::size_t size, smile::memory::Allocator &allocator ) noexcept
{
    return allocator.CreateByteArray( static_cast< smile::Uint32 >( size ) );
}