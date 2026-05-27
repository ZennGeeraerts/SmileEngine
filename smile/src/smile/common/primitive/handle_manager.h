/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "handle.h"
#include "collection/vector.h"

#include <limits>

namespace smile::primitive
{
    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16, typename Tag = void >
    class HandleManager final
    {
      public:
        using HandleType = Handle< IndexType, IndexBits, GenerationBits, Tag >;

        struct Iterator final
        {
            Iterator( const primitive::Vector< HandleType > &handles,
                typename primitive::Vector< HandleType >::ConstIterator it ) noexcept
                : Handles{ handles }, It{ it }
            {
            }

            [[nodiscard]] const HandleType &operator*() const noexcept
            {
                return *It;
            }

            bool operator==( const Iterator &other ) const noexcept
            {
                return It == other.It;
            }

            bool operator!=( const Iterator &other ) const noexcept
            {
                return It != other.It;
            }

            Iterator &operator++()
            {
                do
                {
                    ++It;
                } while ( It != Handles.end() && ( It->GetIndex() > Handles.GetItemCount() ||
                                                     Handles[It->GetIndex()].GetIndex() != It->GetIndex() ) );

                return *this;
            }

            const primitive::Vector< HandleType > &Handles;
            typename primitive::Vector< HandleType >::ConstIterator It;
        };

      public:
        HandleManager() = default;

        HandleType CreateHandle() noexcept
        {
            if ( m_AvailableHandles > 0 )
            {
                // Recycle handle
                auto &handle = m_Handles[m_NextFreeIndex];

                IndexType newHandleIndex = handle.m_Index;
                std::swap( m_NextFreeIndex, newHandleIndex );
                handle.m_Index = newHandleIndex;

                --m_AvailableHandles;

                return handle;
            }
            else
            {
                // Create new handle
                HandleType handle{ static_cast< IndexType >( m_Handles.GetItemCount() ), 0 };
                m_Handles.PushBack( handle );
                return handle;
            }
        }

        void DestroyHandle( HandleType handle ) noexcept
        {
            auto &managedHandle = m_Handles[handle.m_Index];

            SM_ASSERT_MSG( managedHandle.m_Generation == handle.m_Generation,
                "HandleManager::DestroyHandle > Handle generation mismatch" );

            ++managedHandle.m_Generation;

            IndexType newHandleIndex = managedHandle.m_Index;
            std::swap( m_NextFreeIndex, newHandleIndex );
            managedHandle.m_Index = newHandleIndex;

            ++m_AvailableHandles;
        }

        [[nodiscard]] bool IsHandleActive( HandleType handle ) const noexcept
        {
            if ( !handle.IsValid() )
                return false;

            return handle == m_Handles[handle.m_Index];
        }

        [[nodiscard]] HandleType GetHandle( IndexType index ) const noexcept
        {
            SM_ASSERT_MSG( index < m_Handles.GetItemCount(), "HandleManager::GetHandle > Index out of range" );
            return m_Handles[index];
        }

        [[nodiscard]] Count GetHandleCount() const noexcept
        {
            return m_Handles.GetItemCount();
        }

        Iterator begin() const
        {
            auto it = m_Handles.begin();
            while ( it != m_Handles.end() && ( it->GetIndex() > m_Handles.GetItemCount() ||
                                                 m_Handles[it->GetIndex()].GetIndex() != it->GetIndex() ) )
            {
                ++it;
            }

            return { m_Handles, it };
        }

        Iterator end() const
        {
            return { m_Handles, m_Handles.end() };
        }

      private:
        primitive::Vector< HandleType > m_Handles{};
        Count m_AvailableHandles{ 0 };
        IndexType m_NextFreeIndex{ std::numeric_limits< IndexType >::max() }; // Used for implicit list
    };
}