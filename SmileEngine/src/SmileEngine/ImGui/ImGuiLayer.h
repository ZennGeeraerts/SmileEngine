#pragma once
#include "SmileEngine/Core/Layer.h"

#include "SmileEngine/Events/MouseEvent.h"
#include "SmileEngine/Events/ApplicationEvent.h"
#include "SmileEngine/Events/KeyEvent.h"

namespace Smile
{
	class ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetDarkThemeColors();

	private:
		void ShutDown();

		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool m_bInitialized = false;
	};
}

