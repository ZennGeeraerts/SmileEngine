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
 * @file        binding_layout.h
 * @author      Zenn Geeraerts
 * @created     24 April 2026
 * @brief       Describes the layout of resources that can be bound to a shader
 */
#pragma once

#include "smile/graphic/rhi/shader/binding_layout.h"

namespace smile::graphic
{
    class BindingLayout final
    {
      public:
        BindingLayout() = default;

        BindingLayout( rhi::BindingLayoutHandle handle ) noexcept : m_Handle{ handle }
        {
        }

        BindingLayout( const BindingLayout & ) = default;
        BindingLayout( BindingLayout && ) noexcept = default;
        ~BindingLayout() = default;

        BindingLayout &operator=( const BindingLayout & ) = default;
        BindingLayout &operator=( BindingLayout && ) noexcept = default;

        [[nodiscard]] bool IsValid() const noexcept
        {
            return m_Handle.IsValid();
        }

        rhi::BindingLayoutHandle GetHandle() const noexcept
        {
            return m_Handle;
        }

        bool operator==( const BindingLayout &other ) const noexcept
        {
            return m_Handle == other.m_Handle;
        }

      private:
        rhi::BindingLayoutHandle m_Handle;

        friend class ResourceManager;
    };
}