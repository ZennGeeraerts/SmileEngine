#pragma once

#include "smile_engine/core/core.h"
#include "smile_engine/core/events/event.h"

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
