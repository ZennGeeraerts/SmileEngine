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
 * @brief       Abstract base class for providing an instance of a resource
 */
#pragma once

#include "smile/common/foundation/compiled.h"
#include "smile/common/thread/instance_provider_handle.h"

namespace smile::thread
{
    class InstanceProvider
    {
      public:
        InstanceProvider() = default;

        InstanceProviderHandle Get();
        void Release( const InstanceProviderHandle &handle );

      protected:
        ~InstanceProvider() = default;

      private:
        std::atomic< Uint32 > m_Used{ 0 };
    };
}