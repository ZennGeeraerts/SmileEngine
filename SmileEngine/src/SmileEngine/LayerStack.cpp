#include "smpch.h"
#include "LayerStack.h"

namespace Smile
{
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_pLayers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* pLayer : m_pLayers)
			delete pLayer;
	}

	// Insert at m_LayerInsert
	void LayerStack::PushLayer(Layer* pLayer)
	{
		m_LayerInsert = m_pLayers.emplace(m_LayerInsert, pLayer);
		pLayer->OnAttach();
	}

	// Overlays get pushed at the back of the vector
	void LayerStack::PushOverlay(Layer* pOverlay)
	{
		m_pLayers.emplace_back(pOverlay);
		pOverlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* pLayer)
	{
		auto it = std::find(m_pLayers.begin(), m_pLayers.end(), pLayer);
		if (it != m_pLayers.end())
		{
			m_pLayers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* pOverlay)
	{
		auto it = std::find(m_pLayers.begin(), m_pLayers.end(), pOverlay);
		if (it != m_pLayers.end())
			m_pLayers.erase(it);
	}
}