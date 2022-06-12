#pragma once

#include "smile_engine/core/core.h"
#include "layer.h"

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

        void pushLayer( Layer *layer );
        void pushOverlay( Layer *overlay );
        void popLayer( Layer *layer );
        void popOverlay( Layer *overlay );

        std::vector< Layer * >::iterator begin()
        {
            return layers.begin();
        }
        std::vector< Layer * >::iterator end()
        {
            return layers.end();
        }

      private:
        // Vector is used because we iterate over it every frame
        std::vector< Layer * > layers;
        Uint32 layerInsertIndex = 0;
    };
}
