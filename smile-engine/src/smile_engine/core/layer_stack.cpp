#include "smpch.h"
#include "layer_stack.h"

namespace smile
{
    LayerStack::LayerStack()
    {
    }

    LayerStack::~LayerStack()
    {
        for ( Layer *layer : layers )
            delete layer;
    }

    // Insert at layerInsertIndex
    void LayerStack::pushLayer( Layer *layer )
    {
        layers.emplace( layers.begin() + layerInsertIndex, layer );
        ++layerInsertIndex;
        layer->onAttach();
    }

    // Overlays get pushed at the back of the vector
    void LayerStack::pushOverlay( Layer *overlay )
    {
        layers.emplace_back( overlay );
        overlay->onAttach();
    }

    void LayerStack::popLayer( Layer *layer )
    {
        auto it = std::find( layers.begin(), layers.end(), layer );
        if ( it != layers.end() )
        {
            layers.erase( it );
            --layerInsertIndex;
        }
    }

    void LayerStack::popOverlay( Layer *overlay )
    {
        auto it = std::find( layers.begin(), layers.end(), overlay );
        if ( it != layers.end() )
            layers.erase( it );
    }
}