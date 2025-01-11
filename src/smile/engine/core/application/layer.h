/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#pragma once

#include "engine/common/compiled/compiled.h"
#include "engine/core/window/events/event.h"

namespace smile::application
{
    class Layer
    {
      public:
        Layer( const std::string &name = "Layer" );
        virtual ~Layer();

        virtual void OnAttach()
        {
        }
        virtual void OnDetach()
        {
        }
        virtual void OnUpdate( primitive::Timestep deltaTime )
        {
        }
        virtual void OnImGuiRender()
        {
        }
        virtual void OnEvent( window::Event &event )
        {
        }

#ifdef SM_C_DEBUG
        inline const std::string &GetName() const
        {
            return m_DebugName;
        }
#else
        inline const std::string &GetName() const
        {
            return "";
        }
#endif

      protected:
#ifdef SM_C_DEBUG
        std::string m_DebugName;
#endif
    };
}
