/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/foundation/range_iterator.h"
#include "smile/common/primitive/collection/array.h"
#include "smile/common/memory/memory.h"

#include <thread>

namespace smile::thread
{
    template < typename ItemType, Count ThreadCount, Count ItemCount >
    class AggregatedQueue final
    {
      public:
        AggregatedQueue()
        {
            memory::FillByteArray(
                m_ThreadDataUsage.GetData(), 0, m_ThreadDataUsage.GetItemCount() * sizeof( Uint32 ) );
        }

        ~AggregatedQueue() = default;

        AggregatedQueue( const AggregatedQueue & ) = delete;
        AggregatedQueue( AggregatedQueue && ) = delete;

        AggregatedQueue &operator=( const AggregatedQueue & ) = delete;
        AggregatedQueue &operator=( AggregatedQueue && ) = delete;

        bool IsEmpty() const
        {
            for ( const auto &threadData : m_ThreadDataArray )
            {
                if ( !threadData.IsEmpty() )
                    return false;
            }

            return true;
        }

        Count GetItemCount() const
        {
            Count totalCount = 0;

            for ( const auto &threadData : m_ThreadDataArray )
            {
                totalCount += threadData.GetItemCount();
            }

            return totalCount;
        }

        BoolResult Push( const ItemType &item )
        {
            if ( !m_pThreadData->IsFull() )
            {
                m_pThreadData->Items[m_pThreadData->WriteIndex % ItemCount] = item;

                std::atomic_thread_fence( std::memory_order_release );
                ++m_pThreadData->WriteIndex;

                return BoolResult::Succeed();
            }

            return BoolResult::Fail( "Not enough slots available" );
        }

        bool Pop( ItemType &item )
        {
            SM_ASSERT( std::this_thread::get_id() == m_ReaderThreadId );

            Index currentTableIndex = m_LastTableIndex;

            do
            {
                currentTableIndex = ( currentTableIndex + 1 ) % ThreadCount;
                ThreadData *pData = m_pThreadData[currentTableIndex];

                if ( !pData->IsEmpty() )
                {
                    item = pData->Items[pData->ReadIndex % ItemCount];

                    std::atomic_thread_fence( std::memory_order_acquire );
                    ++pData->ReadIndex;

                    m_LastTableIndex = currentTableIndex;

                    return true;
                }
            } while ( currentTableIndex != m_LastTableIndex );

            return false;
        }

        void InitializeReaderThread()
        {
            SM_ASSERT( m_ReaderThreadId == std::thread::id{} );

            m_ReaderThreadId = std::this_thread::get_id();
        }

        void RegisterWriterThread()
        {
            SM_ASSERT( m_pThreadData == nullptr );

            for ( auto dataIndex : foundation::GetCountIterator( m_ThreadDataUsage ) )
            {
                const Uint32 expectedValue{ 0 };

                if ( ( m_ThreadDataUsage[dataIndex] == 0 ) &&
                     m_ThreadDataUsage[dataIndex].compare_exchange_strong( expectedValue, 1 ) )
                {
                    m_pThreadData = &m_ThreadDataArray[dataIndex];
                    return;
                }
            }

            SM_ASSERT_MSG( false, "AggregatedQueue::RegisterWriterThread > No free buffer left" );
        }

        void UnregisterWriterThread()
        {
            if ( !m_pThreadData )
            {
                SM_LOG_WARNING( "AggregatedQueue::UnregisterWriterThread > Thread was not registered" );
                return;
            }

            for ( auto dataIndex : foundation::GetCountIterator( m_ThreadDataArray ) )
            {
                if ( &m_ThreadDataArray[dataIndex] == m_pThreadData )
                {
                    m_ThreadDataUsage[dataIndex] = 0;
                    m_pThreadData = nullptr;
                    return;
                }
            }

            SM_LOG_WARNING( "AggregatedQueue::UnregisterWriterThread > Thread data was not found" );
        }

      private:
        struct ThreadData final
        {
            ThreadData() = default;

            bool IsFull() const
            {
                SM_ASSERT( WriteIndex >= ReadIndex );

                return ( WriteIndex - ReadIndex ) == ItemCount;
            }

            bool IsEmpty() const
            {
                return WriteIndex == ReadIndex;
            }

            Count GetItemCount() const
            {
                SM_ASSERT( WriteIndex >= ReadIndex );

                return WriteIndex - ReadIndex;
            }

            Index ReadIndex{ 0 };
            Index WriteIndex{ 0 };
            primitive::Array< ItemType, ItemCount > Items;
        };

        Index m_LastTableIndex{ 0 };
        primitive::Array< ThreadData, ThreadCount > m_ThreadDataArray;
        std::thread::id m_ReaderThreadId;
        thread_local ThreadData *m_pThreadData;
        primitive::Array< std::atomic< Uint32 >, ThreadCount > m_ThreadDataUsage;
    };
}