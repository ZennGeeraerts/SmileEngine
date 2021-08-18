#pragma once
#include "SmileEngine/Layer.h"

#include "SmileEngine/Events/MouseEvent.h"
#include "SmileEngine/Events/ApplicationEvent.h"
#include "SmileEngine/Events/KeyEvent.h"

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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
	};
}

