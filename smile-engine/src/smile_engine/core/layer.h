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

        virtual void onAttach()
        {
        }
        virtual void onDetach()
        {
        }
        virtual void onUpdate( Timestep delta_time )
        {
        }
        virtual void onImGuiRender()
        {
        }
        virtual void onEvent( Event &event )
        {
        }

        inline const std::string &getName() const
        {
            return debugName;
        }

      protected:
        std::string debugName;
    };
}
