/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2026 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        sorted_command_buffer.h
 * @author      Zenn Geeraerts
 * @created     2 July 2026
 * @brief       Sorted command buffer sorts and groups draw commands by sort key.
 */
#pragma once

namespace smile::graphic
{
    template < typename SortedItem >
    class SortedCommandBuffer final
    {
      public:
        SortedCommandBuffer() = default;
        ~SortedCommandBuffer() = default;

        SortedCommandBuffer( const SortedCommandBuffer & ) = delete;
        SortedCommandBuffer &operator=( const SortedCommandBuffer & ) = delete;

        SortedCommandBuffer( SortedCommandBuffer && ) = default;
        SortedCommandBuffer &operator=( SortedCommandBuffer && ) = default;

        void Add( const SortedItem &item )
        {
            m_Items.PushBack( item );
            m_Sorted = false;
        }

        void Sort()
        {
            if ( m_Sorted )
                return;

            std::sort( m_Items.begin(),
                m_Items.end(),
                []( const SortedItem &a, const SortedItem &b ) { return a.GetSortKey() < b.GetSortKey(); } );

            m_Sorted = true;
        }

        auto begin() noexcept
        {
            Sort();
            return m_Items.begin();
        }

        auto end() noexcept
        {
            Sort();
            return m_Items.end();
        }

        auto begin() const noexcept
        {
            Sort();
            return m_Items.begin();
        }

        auto end() const noexcept
        {
            Sort();
            return m_Items.end();
        }

        Count GetItemCount() const noexcept
        {
            return m_Items.GetItemCount();
        }

        bool IsEmpty() const noexcept
        {
            return m_Items.IsEmpty();
        }

        void Clear() noexcept
        {
            m_Items.Clear();
            m_Sorted = false;
        }

      private:
        primitive::Vector< SortedItem > m_Items;
        bool m_Sorted = false;
    };

    template < typename SortedItem >
    class ViewSortedCommandBuffers final
    {
      public:
        ViewSortedCommandBuffers() = default;
        ~ViewSortedCommandBuffers() = default;

        ViewSortedCommandBuffers( const ViewSortedCommandBuffers & ) = delete;
        ViewSortedCommandBuffers &operator=( const ViewSortedCommandBuffers & ) = delete;

        ViewSortedCommandBuffers( ViewSortedCommandBuffers && ) = default;
        ViewSortedCommandBuffers &operator=( ViewSortedCommandBuffers && ) = default;

        SortedCommandBuffer< SortedItem > &operator[]( const smile::ecs::EntityHandle viewEntity )
        {
            return m_BufferPerView[viewEntity];
        }

        void Clear() noexcept
        {
            for ( const auto &[viewEntity, buffer] : m_BufferPerView )
                buffer.Clear();
        }

      private:
        primitive::HashMap< smile::ecs::EntityHandle, SortedCommandBuffer< SortedItem > > m_BufferPerView;
    };
}