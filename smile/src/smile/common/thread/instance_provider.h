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
 * @file        instance_provider.h
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Concrete templated class for providing an instance of a resource
 */
#pragma once

#include "private/instance_provider_base.h"
#include "smile/common/primitive/collection/array.h"

namespace smile::thread
{
    template < typename HandleType, typename Type >
    class InstanceProvider final : protected InstanceProviderBase
    {
      public:
        template < typename... Args >
        HandleType Get( Args &&...args )
        {
            auto handle = InstanceProviderBase::Get();

            return { m_Instances[handle.Idx], std::move( handle ), std::forward< Args && >( args )... };
        }

      private:
        primitive::Array< Type, 32 > m_Instances;
    };
}