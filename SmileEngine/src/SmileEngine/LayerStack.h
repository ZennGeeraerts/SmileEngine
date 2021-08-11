#pragma once

#include "SmileEngine/Core.h"
#include "Layer.h"

namespace Smile
{
	class SMILE_API LayerStack final
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* pLayer);
		void PushOverlay(Layer* pOverlay);
		void PopLayer(Layer* pLayer);
		void PopOverlay(Layer* pOverlay);

		std::vector<Layer*>::iterator begin() { return m_pLayers.begin(); }
		std::vector<Layer*>::iterator end() { return m_pLayers.end(); }

	private:
		// Vector is used because we iterate over it every frame
		std::vector<Layer*> m_pLayers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}

