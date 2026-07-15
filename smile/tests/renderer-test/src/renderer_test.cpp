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
 * @file        renderer_test.cpp
 * @author      Zenn Geeraerts
 * @created     10 March 2026
 * @brief       Renderer test application
 */

#include "smile/common/platform/executable_path.h"
#include "smile/core/application/entry_point.h"
#include "smile/core/application/application.h"
#include "renderer_test_layer.h"

namespace smile
{
    class RendererTest final : public application::Application
    {
      public:
        RendererTest( const application::ApplicationDescriptor &descriptor ) noexcept
            : application::Application{ descriptor }
        {
        }

        void Initialize() override
        {
            Application::Initialize();

            PushLayer( new graphic::RendererTestLayer{} );
        }
    };

    application::Application *application::CreateApplication( application::ApplicationCommandLineArgs commandLineArgs )
    {
        smile::application::ApplicationDescriptor descriptor{};
        descriptor.Name = "Renderer Test";
        descriptor.WorkingDirectory = platform::GetExecutablePath().GetDirectory();
        descriptor.CommandLineArgs = commandLineArgs;

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new RendererTest{ descriptor };
    }
}