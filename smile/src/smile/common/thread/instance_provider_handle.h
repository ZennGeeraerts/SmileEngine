/*=======================================================================
*    _____           _ _          |                                     *
*   / ____|         (_) |         |                                     *
*  | (___  _ __ ___  _| | ___     |                                     *
*   \___ \| '_ ` _ \| | |/ _ \    |  Copyright (c) 2025 Smile Engine    *
*   ____) | | | | | | | |  __/    |  Inc. All Rights Reserved           *
*  |_____/|_| |_| |_|_|_|\___|    |                                     *
*                                 |                                     *
=======================================================================*/

/**
 * @file        instance_provider_handle.h
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Handle used for provided instances
 */
#pragma once

#include "smile/common/foundation/compiled.h"

namespace smile::thread
{
    class InstanceProvider;

    class InstanceProviderHandle final
    {
      public:
        InstanceProviderHandle( InstanceProvider *pProvider, const Index index ) noexcept
            : m_pProvider{ pProvider }, Idx{ index }
        {
        }

        ~InstanceProviderHandle() noexcept;

        InstanceProviderHandle( InstanceProviderHandle &&other ) noexcept
            : Idx{ other.Idx }, m_pProvider{ other.m_pProvider }
        {
            other.m_pProvider = nullptr;
        }

        InstanceProviderHandle( const InstanceProviderHandle & ) = delete;

        InstanceProviderHandle &operator=( const InstanceProviderHandle & ) = delete;
        InstanceProviderHandle &operator=( InstanceProviderHandle && ) = delete;

        const Index Idx;

      private:
        InstanceProvider *m_pProvider;
    };
}