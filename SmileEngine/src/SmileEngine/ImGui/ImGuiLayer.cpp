#include "smpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"

#include "SmileEngine/SmileGame.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DirectX11Context.h"

#include "SmileEngine/Logger.h"

namespace Smile
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Smile Engine keycodes
		io.KeyMap[ImGuiKey_Tab] = VK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Space] = VK_SPACE;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';

		Window& window = SmileGame::GetInstance().GetWindow();
		ImGui_ImplWin32_Init(window.GetNativeWindow());

		RenderingContext* pRenderingContext = window.GetRenderingContext();
		DirectX11Context* pD11Context = dynamic_cast<DirectX11Context*>(pRenderingContext);

		if (pD11Context)
			ImGui_ImplDX11_Init(pD11Context->GetDevice(), pD11Context->GetDeviceContext());
		else
			SM_LOG_ERROR("ImGuiLayer::OnAttach > ImGuiLayer currently only supports DirectX11");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Window& window = SmileGame::GetInstance().GetWindow();
		io.DisplaySize = ImVec2{ static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()) };
		io.DeltaTime = SmTime::GetInstance().GetDeltaTime();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool bShow = true;
		ImGui::ShowDemoWindow(&bShow);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher{ event };
		dispatcher.Dispatch<MouseButtonPressedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolled));
		dispatcher.Dispatch<KeyPressedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
		dispatcher.Dispatch<KeyTypedEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));
		dispatcher.Dispatch<WindowResizeEvent>(SM_BIND_EVENT_FN(ImGuiLayer::OnWindowResize));
	}

	bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2{ e.GetX(), e.GetY() };

		return false;
	}

	bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetOffsetX();
		io.MouseWheelH += e.GetOffsetY();

		return false;
	}

	bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[VK_LCONTROL] || io.KeysDown[VK_RCONTROL];
		io.KeyShift = io.KeysDown[VK_LSHIFT] || io.KeysDown[VK_RSHIFT];
		io.KeyAlt = io.KeysDown[VK_LMENU] || io.KeysDown[VK_RMENU];
		io.KeySuper = io.KeysDown[VK_LWIN] || io.KeysDown[VK_RWIN];
		return false;
	}

	bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if ((keycode > 0) && (keycode < 0x10000))
			io.AddInputCharacter(static_cast<unsigned short>(keycode));

		return false;
	}

	bool ImGuiLayer::OnWindowResize(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2{ static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()) };
		io.DisplayFramebufferScale = ImVec2{ 1.f, 1.f };

		// TODO: Update window size and DirectXContext

		return false;
	}
}