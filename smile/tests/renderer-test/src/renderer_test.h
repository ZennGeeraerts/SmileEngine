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
 * @file        renderer_test.h
 * @author      Zenn Geeraerts
 * @created     10 March 2026
 * @brief       Renderer test application
 */
#pragma once

#include "smile/core/application/application.h"

namespace smile
{
    class RendererTest final : public application::Application
    {
      public:
        RendererTest( const application::ApplicationDescriptor &descriptor );
    };
}