/*=============================================================================*/
// Copyright 2022-2025 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "rhi_test.h"

#include "smile/core/application/entry_point.h"
#include "rhi_test_layer.h"

namespace smile
{
    application::Application *application::CreateApplication( application::ApplicationCommandLineArgs commandLineArgs )
    {
        smile::application::ApplicationDescriptor descriptor{};
        descriptor.Name = "RHI Test";
        descriptor.WorkingDirectory = ".";
        descriptor.CommandLineArgs = commandLineArgs;

        // This application will get passed to the entry point of the engine
        // and will be deleted once the engine closes
        return new RHITest{ descriptor };
    }

    RHITest::RHITest( const application::ApplicationDescriptor &descriptor ) : application::Application{ descriptor }
    {
        PushLayer( new RHITestLayer{} );
    }
}