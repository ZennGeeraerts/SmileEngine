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
 * @file        draw_command_buffer.h
 * @author      Zenn Geeraerts
 * @created     01 June 2026
 * @brief       Sorted, per-pass list of DrawCommands.
 *
 *              DrawCommandBuffer accumulates DrawCommand entries submitted by scene
 *              extraction, then sorts them ascending by SortKey immediately before
 *              the render-graph pass execute lambda runs. This ensures state-minimal
 *              GPU dispatch (opaque: near-to-far within PSO/material groups;
 *              transparent: far-to-near).
 *
 *              The buffer owns its storage via a primitive::Vector. Callers reset
 *              it each frame via Clear(); no heap deallocation occurs on reset.
 *
 *              Thread safety: not thread-safe. Populate from a single thread.
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/primitive/collection/vector.h"
#include "smile/common/primitive/collection/array_view.h"
#include "smile/graphic/renderer/draw/draw_command.h"

namespace smile::graphic
{
    class DrawCommandBuffer final
    {
      public:
        DrawCommandBuffer() = default;
        ~DrawCommandBuffer() = default;

        DrawCommandBuffer( const DrawCommandBuffer & ) = delete;
        DrawCommandBuffer &operator=( const DrawCommandBuffer & ) = delete;

        DrawCommandBuffer( DrawCommandBuffer && ) = default;
        DrawCommandBuffer &operator=( DrawCommandBuffer && ) = default;

        /**
         * Appends @p command to the buffer and returns a reference to the stored entry.
         * The reference is valid until the next call to Clear().
         */
        DrawCommand &Emplace( DrawCommand command )
        {
            m_Commands.PushBack( std::move( command ) );
            m_Sorted = false;
            return m_Commands.GetLastItem();
        }

        /**
         * Sorts all commands ascending by SortKey. Idempotent if the buffer has not
         * been modified since the last Sort() call.
         */
        void Sort();

        /**
         * Returns a view of all commands in the buffer. Commands are in insertion
         * order until Sort() is called, then in ascending SortKey order.
         */
        [[nodiscard]] primitive::ArrayView< const DrawCommand > GetCommands() const noexcept
        {
            return { m_Commands.GetData(), m_Commands.GetItemCount() };
        }

        /** Number of commands currently in the buffer. */
        [[nodiscard]] Count GetCommandCount() const noexcept
        {
            return m_Commands.GetItemCount();
        }

        /** True if the buffer has no commands. */
        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return m_Commands.IsEmpty();
        }

        /**
         * Removes all commands. Keeps the underlying storage allocated for reuse next frame.
         */
        void Clear() noexcept
        {
            m_Commands.Clear();
            m_Sorted = false;
        }

      private:
        primitive::Vector< DrawCommand > m_Commands;
        bool m_Sorted = false;
    };
}
