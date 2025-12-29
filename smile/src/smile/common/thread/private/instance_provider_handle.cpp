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
 * @file        instance_provider_handle.cpp
 * @author      Zenn Geeraerts
 * @created     29 December 2025
 * @brief       Handle used for provided instances
 */
#include "smpch.h"
#include "smile/common/thread/instance_provider_handle.h"

#include "instance_provider.h"

namespace smile::thread
{
    InstanceProviderHandle::~InstanceProviderHandle() noexcept
    {
        if ( m_pProvider )
        {
            m_pProvider->Release( *this );
        }
    }
}