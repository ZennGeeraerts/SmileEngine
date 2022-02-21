#pragma once

#include "SmileEngine/Core/Core.h"
#include "SmileEngine/Events/Event.h"

namespace smile
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
        virtual void OnUpdate( Timestep deltaTime )
        {
        }
        virtual void OnImGuiRender()
        {
        }
        virtual void OnEvent( Event &event )
        {
        }

        inline const std::string &GetName() const
        {
            return m_DebugName;
        }

      protected:
        std::string m_DebugName;
    };
}
