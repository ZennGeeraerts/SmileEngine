/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "handle.h"

#include <vector>
#include <limits>

namespace smile::primitive
{
    template < typename IndexType = Uint32, Uint32 IndexBits = 16, Uint32 GenerationBits = 16 >
    class HandleManager final
    {
      public:
        using HandleType = Handle< IndexType, IndexBits, GenerationBits >;

        struct Iterator final
        {
            Iterator( const std::vector< HandleType > &handles, typename std::vector< HandleType >::const_iterator it )
                : Handles{ handles }, It{ it }
            {
            }

            HandleType operator*() const
            {
                return *It;
            }
            bool operator==( const Iterator &other ) const
            {
                return It == other.It;
            }
            bool operator!=( const Iterator &other ) const
            {
                return It != other.It;
            }

            Iterator &operator++()
            {
                do
                {
                    ++It;
                } while ( It != Handles.end() &&
                          ( It->GetIndex() > Handles.size() || Handles[It->GetIndex()].GetIndex() != It->GetIndex() ) );

                return *this;
            }

            const std::vector< HandleType > &Handles;
            typename std::vector< HandleType >::const_iterator It;
        };

      public:
        HandleManager() = default;

        HandleType CreateHandle()
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
                HandleType handle{ static_cast< IndexType >( m_Handles.size() ), 0 };
                m_Handles.push_back( handle );
                return handle;
            }
        }

        void DestroyHandle( HandleType handle )
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

        bool IsHandleActive( HandleType handle ) const
        {
            if ( !handle.IsValid() )
                return false;

            return handle == m_Handles[handle.m_Index];
        }

        HandleType GetHandle( IndexType index ) const
        {
            SM_ASSERT_MSG( index < m_Handles.size(), "HandleManager::GetHandle > Index out of range" );
            return m_Handles[index];
        }

        Uint32 GetHandleCount() const
        {
            return static_cast< Uint32 >( m_Handles.size() );
        }

        Iterator begin() const
        {
            auto it = m_Handles.begin();
            while ( it != m_Handles.end() &&
                    ( it->GetIndex() > m_Handles.size() || m_Handles[it->GetIndex()].GetIndex() != it->GetIndex() ) )
            {
                ++it;
            }

            return Iterator{ m_Handles, it };
        }
        Iterator end() const
        {
            return Iterator{ m_Handles, m_Handles.end() };
        }

      private:
        std::vector< HandleType > m_Handles{};
        Uint32 m_AvailableHandles{ 0 };
        IndexType m_NextFreeIndex{ std::numeric_limits< IndexType >::max() }; // Used for implicit list
    };
}