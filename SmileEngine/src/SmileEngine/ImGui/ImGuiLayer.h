#pragma once
#include "SmileEngine/Layer.h"

#include <d3d11.h>

namespace Smile
{
	class SMILE_API ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
	};
}

