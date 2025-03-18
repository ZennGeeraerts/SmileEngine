/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "layer_stack.h"

namespace smile::application
{
    LayerStack::LayerStack()
    {
    }

    LayerStack::~LayerStack()
    {
        for ( Layer *pLayer : m_pLayers )
        {
            pLayer->OnDetach();
            delete pLayer;
        }
    }

    // Insert at layerInsertIndex
    void LayerStack::PushLayer( Layer *pLayer )
    {
        m_pLayers.emplace( m_pLayers.begin() + m_LayerInsertIndex, pLayer );
        ++m_LayerInsertIndex;
        pLayer->OnAttach();
    }

    // Overlays get pushed at the back of the vector
    void LayerStack::PushOverlay( Layer *pOverlay )
    {
        m_pLayers.emplace_back( pOverlay );
        pOverlay->OnAttach();
    }

    void LayerStack::PopLayer( Layer *pLayer )
    {
        auto it = std::find( m_pLayers.begin(), m_pLayers.end(), pLayer );
        if ( it != m_pLayers.end() )
        {
            pLayer->OnDetach();
            m_pLayers.erase( it );
            --m_LayerInsertIndex;
        }
    }

    void LayerStack::PopOverlay( Layer *pOverlay )
    {
        auto it = std::find( m_pLayers.begin(), m_pLayers.end(), pOverlay );
        if ( it != m_pLayers.end() )
        {
            pOverlay->OnDetach();
            m_pLayers.erase( it );
        }
    }
}