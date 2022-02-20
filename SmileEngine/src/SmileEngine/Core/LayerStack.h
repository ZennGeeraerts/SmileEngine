#pragma once

#include "SmileEngine/Core/Core.h"
#include "Layer.h"

namespace smile
{
    class LayerStack final
    {
      public:
        LayerStack();
        ~LayerStack();

        LayerStack( const LayerStack & ) = delete;
        LayerStack( LayerStack && ) = delete;
        LayerStack &operator=( const LayerStack & ) = delete;
        LayerStack &operator=( LayerStack && ) = delete;

        void PushLayer( Layer *pLayer );
        void PushOverlay( Layer *pOverlay );
        void PopLayer( Layer *pLayer );
        void PopOverlay( Layer *pOverlay );

        std::vector< Layer * >::iterator begin()
        {
            return m_pLayers.begin();
        }
        std::vector< Layer * >::iterator end()
        {
            return m_pLayers.end();
        }

      private:
        // Vector is used because we iterate over it every frame
        std::vector< Layer * > m_pLayers;
        Uint32 m_LayerInsertIndex = 0;
    };
}
